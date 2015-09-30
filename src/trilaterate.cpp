/*
 * Trilateration
 *
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


class MyCostFunctor{

    public:

        MyCostFunctor(vector<double> bi_, double mi_)
            : bi(bi_), mi(mi_) {}

        //minimize (a-b^2)^2 instead of (sqrt(a)-b)^2
        template <typename T>
        bool operator()(const T* const pos, T* residual) const {

            T square_sum = T(0);

            for (int i = 0; i < bi.size(); ++i) {
                square_sum += pow(pos[i]-T(bi[i]), 2);
            }

            residual[0] = square_sum - T(mi) * T(mi);

            return true;
        }


    private:
        const vector<double> bi;
        const double mi;
};


int main(int argc, char** argv) {

    Point<double> target;
    vector< Point<double> > beacon;

    double std_dev = 0.1;

    // Parse arguments
    // TODO check if input is well formed
    bool valid_input = false;
    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev = atof(argv[++i]);

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
    cout << "Target is in " <<  target.toString() << endl;

    vector< double > measures; // distance between target and beacon + gaussian noise

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0, std_dev);


    for (int i=0; i<beacon.size(); ++i){
        double dist = target.distanceTo(beacon[i]);
        double noise = distribution(generator);

        measures.push_back(dist + noise);

        cout << "Beacon " << i << ": " << beacon[i].toString() << "\t| distance(" << dist
             << ") + noise (" << noise << ")\t= " << dist + noise << endl;
    }
    cout << "------------------------------------------------------------------\n\n";

    google::InitGoogleLogging(argv[0]);


    // The variable to solve for with its initial value. It will be
    // mutated in place by the solver.
    Point<double> initial_guess(1000.0, 1000.0, -80.0);
    double est_coords[] = { initial_guess.getX(), initial_guess.getY(), initial_guess.getZ()};


    // Build the problem.
    Problem problem;


    for (int i = 0; i < beacon.size(); ++i) {
        vector<double> bi = beacon[i].getCoords();

        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3>(
                    new MyCostFunctor(bi, measures[i]));

        problem.AddResidualBlock(cost_f, NULL, est_coords);
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

    Point<double>target_est(est_coords[0], est_coords[1], est_coords[2]);
    cout << "Estimated position: " << target_est.toString() << endl;
    cout << "The estimated position is far " << target.distanceTo(target_est) << " from the real position\n\n";



    /*
     * TODO check correctness of this matrix
     */
    if(beacon.size() < 3){
        cout << "Minimum 3 beacon are needed for the calculation of covariance matrix\n";
    } else {
        Covariance::Options cov_opt;
        Covariance covariance(cov_opt);

        vector<pair<const double*, const double*> > covariance_blocks;
        covariance_blocks.push_back(make_pair(est_coords, est_coords));

        CHECK(covariance.Compute(covariance_blocks, &problem));

        double covariance_xx[3 * 3];
        covariance.GetCovarianceBlock(est_coords, est_coords, covariance_xx);

        cout << "Covariance Matrix:\n";
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                cout << covariance_xx[3*i + j] << ";  ";
            }
            cout << "\n";
        }
    }

    return 0;

}
