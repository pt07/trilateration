/*
 * Trilateration
 * trilaterate
 * e.g.
 *  ./trilaterate -t 10 10 3 -b 2 4 100 -b 8 30 100 -b 20 10 100
 */

#include "ceres/ceres.h"
#include "glog/logging.h"

#include <iostream>
#include <vector>
#include <string.h>     //strcmp
#include <cstdlib>      //atof
#include <random>       //gaussian random number

#include "Point.h"

using namespace std;

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

using ceres::Covariance;

const double LIGHT_VELOCITY = 3e8; // m/s


class MyCostFunctor{

    public:

        MyCostFunctor(vector<double> bi_, double ti_, double vel_)
            : bi(bi_), ti(ti_), vel(vel_) {}

        //minimize (a-b^2)^2 instead of (sqrt(a)-b)^2
        template <typename T>
        bool operator()(const T* const pos, const T* const bias, T* residual) const {

            T square_sum = T(0);

            for (int i = 0; i < bi.size(); ++i) {
                square_sum += pow(pos[i]-T(bi[i]), 2);
            }

            // TODO check correctness of the formula!!!!
            residual[0] = square_sum - pow(vel*(ti - bias[0]), 2) ;

            return true;
        }


    private:
        const vector<double> bi;
        const double ti;
        const double vel;
};






int main(int argc, char** argv) {

    Point<double> target;
    vector< Point<double> > beacon;

    double velocity = LIGHT_VELOCITY;
    double bias = 2e-7; //bias of the target's clock in seconds
    double std_dev = 1e-8;


    // Parse arguments
    // TODO check if input is well formed
    bool valid_input = false;
    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev = atof(argv[++i]);

        } else if (strcmp (argv[i], "--bias") == 0){

            bias = atof(argv[++i]);

        } else if ((strcmp (argv[i], "--beacon") == 0) || (strcmp (argv[i], "-b") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = 0;
            if(i+1<argc)
                z = atof(argv[i + 1]); //if is text, it goes to 0 by default

            beacon.push_back(Point<double>(x, y, z));

        } else if ((strcmp (argv[i], "--target") == 0) || (strcmp (argv[i], "-t") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = 0;
            if(i+1<argc)
                z = atof(argv[i + 1]); //if is text, it goes to 0 by default

            target.setCoords(x, y, z);
            valid_input = true;
        }
    }

    if( !valid_input || beacon.size()<2){
        cout << "Input is not valid\n";
        cout << "Set the position of the target with '-t x y (z)' and at least 2 beacons with '-b x y (z)'\n";
        return -1;
    }

    cout << "Standard deviation = " << std_dev << endl;
    cout << "Target is in " <<  target.toString() << " and have a time bias of " << bias << endl;


    vector< double > time_measures; // time between target and beacon + target_bias + gaussian noise

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0, std_dev);


    for (int i=0; i<beacon.size(); ++i){
        double time = target.distanceTo(beacon[i]) / velocity;
        double noise = distribution(generator);

        time_measures.push_back(time + bias + noise);

        cout << "Beacon " << i << ": " << beacon[i].toString()
             << "\t | time(" << time
             << ") + bias (" << bias
             << ") + noise (" << noise
             << ")\t= " << time + bias + noise << endl;
    }
    cout << "------------------------------------------------------------------\n\n";

    google::InitGoogleLogging(argv[0]);


    // The variable to solve for with its initial value. It will be
    // mutated in place by the solver.
    Point<double> initial_guess(0.0, 0.0, 0.0);
    double est_coords[] = { initial_guess.getX(), initial_guess.getY(), initial_guess.getZ()};
    double est_bias = 1e-8;


    // Build the problem.
    Problem problem;

    for (int i = 0; i < beacon.size(); ++i) {
        vector<double> bi = beacon[i].getCoords();

        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3, 1>(
                    new MyCostFunctor(bi, time_measures[i], velocity));

        problem.AddResidualBlock(cost_f, NULL, est_coords, &est_bias);
    }


    Solver::Options options;
    options.max_num_iterations = 25;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);
    cout << summary.BriefReport() << "\n\n";




    cout << "Initial guess: " << initial_guess.toString() << endl;
    cout << "Real position: " << target.toString() << endl;
    cout << "Real bias: " << bias << endl;
    cout << "Estimated bias: " << est_bias << endl;

    Point<double>target_est(est_coords[0], est_coords[1], est_coords[2]);
    cout << "Estimated position: " << target_est.toString() << endl;
    cout << "The estimated position is far " << target.distanceTo(target_est) << " from the real position\n\n";









    return 0;

}
