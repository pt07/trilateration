/*
 * Trilateration
 *
 * Uses time measurement instead of spacial.
 * receiver has a time bias.
 * There is gaussian noise on measurements.
 *
 * e.g.
 * ./trilaterate2 -r 3 -3 2 -s 15 26 40 -s 1 -32 50 -s -60 -8 70 -s 24 73 56 -s -99 -66 77
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

const double NANO = 1e-9;
const double SPEED_OF_LIGHT = 3e8; // m / s


class MyCostFunctor{

    public:

        MyCostFunctor(vector<double> sat_i_, double t_i_, double vel_)
            : sat_i(sat_i_), t_i(t_i_), vel(vel_) {}

        template <typename T>
        bool operator()(const T* const pos, const T* const bias, T* residual) const {

            T square_sum = T(0);

            for (int i = 0; i < sat_i.size(); ++i) {
                square_sum += pow(pos[i] - T(sat_i[i]) , 2);
            }

            T distance = (square_sum != T(0)) ? sqrt(square_sum) : T(0) ;

            residual[0] = distance + vel * (bias[0] - t_i);


            /* TODO
             * operare con misure di tempo
             *
             *
             * error = expected measurement - actual measurement
             * con  expected measurement = distance / vel + bias[0]
             *      actual measurement   = t_i
             */

 //           residual[0] = distance / vel + bias[0] - t_i;

            return true;
        }


    private:
        const vector<double> sat_i;
        const double t_i;
        const double vel;
};


int main(int argc, char** argv) {

    // Parameters

    double velocity = SPEED_OF_LIGHT;
    double bias = 100 * NANO; //bias of the receiver's clock in seconds
    //TODO with this values it works good. is std_dev too small?
    double std_dev = 1 * NANO; // std deviation of the gaussian white noise added to measuremens

    // The variable to solve for with its initial value.
    Point<double> initial_coords_guess(0.0, 0.0, 0.0);
    double initial_bias_guess = 120 * NANO;



    Point<double> receiver;
    vector< Point<double> > satellites;


    // Parse arguments
    // TODO check if input is well formed
    bool valid_input = false;
    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev = atof(argv[++i]);

        } else if (strcmp (argv[i], "--bias") == 0  || (strcmp (argv[i], "-b") == 0)){

            bias = atof(argv[++i]);

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
        cout << "Set the position of the receiver with '-t x y (z)' and at least 2 satellites with '-b x y (z)'\n";
        return -1;
    }

    cout << "Standard deviation = " << std_dev << endl;
    cout << "Receiver is in " <<  receiver.toString() << " and have a time bias of " << bias << endl;


    vector< double > time_measures; // time between receiver and satellite + receiver_bias + gaussian noise

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0, std_dev);


    for (size_t i=0; i<satellites.size(); ++i){
        double time = receiver.distanceTo(satellites[i]) / velocity;
        double noise = distribution(generator);

        time_measures.push_back(time + bias + noise);

        cout << "Satellite " << i << ": " << satellites[i].toString()
             << "\t | time (" << time
             << ") + bias (" << bias
             << ") + noise (" << noise
             << ")\t= " << time + bias + noise << " ns" << endl;
    }
    cout << "------------------------------------------------------------------\n\n";

    google::InitGoogleLogging(argv[0]);




    double est_coords[] = { initial_coords_guess.getX(), initial_coords_guess.getY(), initial_coords_guess.getZ()};
    double est_bias = initial_bias_guess;


    // Build the problem.
    Problem problem;

    for (size_t i = 0; i < satellites.size(); ++i) {
        vector<double> sat_i = satellites[i].getCoords();

        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3, 1>(
                    new MyCostFunctor(sat_i, time_measures[i], velocity));

        problem.AddResidualBlock(cost_f, NULL, est_coords, &est_bias);
    }


    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    options.parameter_tolerance = 1e-12;
    options.function_tolerance = 1e-12;
    options.gradient_tolerance = 1e-12;
    options.max_num_iterations = 1000;

    Solver::Summary summary;
    Solve(options, &problem, &summary);
    cout << summary.BriefReport() << "\n\n";



    Point<double>receiver_est(est_coords[0], est_coords[1], est_coords[2]);

    cout << "Initial guess: position" << initial_coords_guess.toString() << "\tbias " << initial_bias_guess << endl;

    cout << "BIAS:\t  real = " << bias
         << "\n\t  estimated = " << est_bias
         << "\n\t  ratio = " << est_bias/bias << endl;

    cout << "POSITION: real: " << receiver.toString()
         << "\n\t  estimated: " << receiver_est.toString()
         << "\n\t  distance: " << receiver.distanceTo(receiver_est) << "\n\n";


    return 0;

}
