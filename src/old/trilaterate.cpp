/*
 * Trilateration based on distance measurements
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
#include <math.h>       //sqrt

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

        MyCostFunctor(vector<double> sat_i_, double meas_i_)
            : sat_i(sat_i_), meas_i(meas_i_) {}

        template <typename T>
        bool operator()(const T* const pos, T* residual) const {

            T square_sum = T(0);

            for (int i = 0; i < sat_i.size(); ++i) {
                square_sum += pow(pos[i]-T(sat_i[i]), 2);
            }

            T distance = (square_sum != T(0)) ? sqrt(square_sum) : T(0) ;

            //error = expected measurement - actual measurement
            residual[0] = distance - T(meas_i);

            return true;
        }


    private:
        const vector<double> sat_i;
        const double meas_i;
};


int main(int argc, char** argv) {

    Point<double> receiver;
    vector< Point<double> > satellites;

    double std_dev = 0.1;

    // Parse arguments
    // TODO check if input is well formed
    bool valid_input = false;
    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev = atof(argv[++i]);

        } else if ((strcmp (argv[i], "--satellite") == 0) || (strcmp (argv[i], "-s") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = 0;
            if(i+1<argc)
                z = atof(argv[i + 1]); //if is text, it goes to 0 by default

            satellites.push_back(Point<double>(x, y, z));

        } else if ((strcmp (argv[i], "--receiver") == 0) || (strcmp (argv[i], "-r") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = 0;
            if(i+1<argc)
                z = atof(argv[i + 1]); //if is text, it goes to 0 by default

            receiver.setCoords(x, y, z);
            valid_input = true;
        }
    }

    if( !valid_input || satellites.size()<2){
        cout << "Input is not valid\n";
        cout << "Set the position of the receiver with '-r x y (z)' and at least 2 satellites with '-s x y (z)'\n";
        return -1;
    }

    cout << "Standard deviation = " << std_dev << endl;
    cout << "Receiver is in " <<  receiver.toString() << endl;

    vector< double > measures; // distance between receiver and satellite + gaussian noise

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0, std_dev);


    for (size_t i=0; i<satellites.size(); ++i){
        double dist = receiver.distanceTo(satellites[i]);
        double noise = distribution(generator);

        measures.push_back(dist + noise);

        cout << "Satellite " << i << ": " << satellites[i].toString() << "\t| distance(" << dist
             << ") + noise (" << noise << ")\t= " << dist + noise << endl;
    }
    cout << "------------------------------------------------------------------\n\n";

    google::InitGoogleLogging(argv[0]);


    // The variable to solve for with its initial value. It will be
    // mutated in place by the solver.
    Point<double> initial_guess(0, 0, 0);
    double est_coords[] = { initial_guess.getX(), initial_guess.getY(), initial_guess.getZ()};


    // Build the problem.
    Problem problem;


    for (size_t i = 0; i < satellites.size(); ++i) {
        vector<double> sat_i = satellites[i].getCoords();

        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3>(
                    new MyCostFunctor(sat_i, measures[i]));

        problem.AddResidualBlock(cost_f, NULL, est_coords);
    }
    Solver::Options options;
    options.max_num_iterations = 250;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    Solver::Summary summary;
    Solve(options, &problem, &summary);
    cout << summary.BriefReport() << "\n\n";


    cout << "Initial guess: " << initial_guess.toString() << endl;
    cout << "Real position: " << receiver.toString() << endl;

    Point<double>receiver_est(est_coords[0], est_coords[1], est_coords[2]);
    cout << "Estimated position: " << receiver_est.toString() << endl;
    cout << "The estimated position is far " << receiver.distanceTo(receiver_est) << " from the real position\n\n";



    /*
     * TODO check correctness of this matrix
     */
    if(satellites.size() < 3){
        cout << "Minimum 3 satellite are needed for the calculation of covariance matrix\n";
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
