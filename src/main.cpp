#include "ceres/ceres.h"
#include "glog/logging.h"

#include <iostream>
#include <vector>
#include <string.h>     //strcmp
#include <cstdlib>      //atof
#include <random>       //gaussian random number

#include "Point.cpp" //TODO sarebbe da importare il .h

using namespace std;

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;


// A templated cost functor that implements the residual r = 10 -
// x. The method operator() is templated so that we can then use an
// automatic differentiation wrapper around it to generate its
// derivatives.
//struct CostFunctor {
//  template <typename T> bool operator()(const T* const x, T* residual) const {
//    residual[0] = T(10.0) - x[0];
//    return true;
//  }
//};

int main(int argc, char** argv) {
    Point<double> receiver;

    double std_dev = 1.0;
    vector< Point<double> > satellites;


    // Parse arguments
    // TODO check if input is well formed
    bool valid_input = false;
    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev = atof(argv[++i]);

        } else if ((strcmp (argv[i], "--satellite") == 0) || (strcmp (argv[i], "-s") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);

            satellites.push_back(Point<double>(x, y));
        } else if ((strcmp (argv[i], "--receiver") == 0) || (strcmp (argv[i], "-r") == 0)){

           receiver.setX( atof(argv[++i]) );
           receiver.setY( atof(argv[++i]) );


            valid_input = true;
        }
    }

    if( !valid_input ){
        cout << "Input is not valid\n";
        cout << "Set the position of the receiver with '-r x y' and at least 3 satellites with '-s x y'\n";
        return -1;
    }

    cout << "Standard deviation = " << std_dev << endl;
    cout << "Receiver is in (" <<  receiver.getX() << ", "  << receiver.getY() << ")\n";


    vector< double > measures; // distance between receiver and satellite + gaussian noise

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0, std_dev);


    for (int i=0; i<satellites.size(); ++i){
        double dist = receiver.distanceTo(satellites[i]);


        double noise = distribution(generator);

        measures.push_back(dist + noise);



        cout << "Satellite " << i << ": (" << satellites[i].getX() << ", "
             << satellites[i].getY() << ")\t | distance: " << dist << "\t--> " << dist + noise
             << "\tnoise=" << noise << endl;
    }
    cout << endl;



//  google::InitGoogleLogging(argv[0]);

//  // The variable to solve for with its initial value. It will be
//  // mutated in place by the solver.
//  double x = 0.5;
//  const double initial_x = x;

//  // Build the problem.
//  Problem problem;

//  // Set up the only cost function (also known as residual). This uses
//  // auto-differentiation to obtain the derivative (jacobian).
//  CostFunction* cost_function =
//      new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
//  problem.AddResidualBlock(cost_function, NULL, &x);

//  // Run the solver!
//  Solver::Options options;
//  options.minimizer_progress_to_stdout = true;
//  Solver::Summary summary;
//  Solve(options, &problem, &summary);

//  std::cout << summary.BriefReport() << "\n";
//  std::cout << "x : " << initial_x
//            << " -> " << x << "\n";
  return 0;
}
