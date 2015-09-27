/*
 * 2D Trilateration
 *
 * e.g.
 * ./trilaterate -t 5.2 5.3 -b 1.2 2.2 -b 9.9 10 -b 0 8 -b 2 3 -b 6 7 -b 10 1 -b 0 15 -d 1
 */

/*
 * prova
 * ora fondo il branch con il master
 */

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

const int DIM = 2; //dimension: 2D or 3D?

//TODO
//TODO  come faccio a usare il template qui dentro?
//TODO  nel senso, tenere le variabili private T e non double
//template <class T>
class MyCostFunctor
{
    public:

        MyCostFunctor(vector<double> bi_, double mi_)
            : bi(bi_), mi(mi_) {

        }

        template <typename T>
        bool operator()(const T* const pos, T* residual) const {

            T square_sum = T(0);

            for (int i = 0; i < bi.size(); ++i) {
                square_sum += pow(pos[i]-T(bi[i]), 2);
            }

            residual[0] = sqrt( square_sum ) - T(mi);
            return true;
        }

    private:
        const vector<double> bi;

        const double mi;
};

// TODO to ask DIFFERENCES?
//struct MeasurementResidual {....};




int main(int argc, char** argv) {

    Point<double> target;

    double std_dev = 0.1;
    vector< Point<double> > beacon;


    // Parse arguments
    // TODO check if input is well formed
    bool valid_input = false;
    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev = atof(argv[++i]);

        } else if ((strcmp (argv[i], "--beacon") == 0) || (strcmp (argv[i], "-b") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);

            beacon.push_back(Point<double>(x, y));
        } else if ((strcmp (argv[i], "--target") == 0) || (strcmp (argv[i], "-t") == 0)){

           target.setX( atof(argv[++i]) );
           target.setY( atof(argv[++i]) );


           valid_input = true;
        }
    }

    if( !valid_input || beacon.size()<2){
        cout << "Input is not valid\n";
        cout << "Set the position of the target with '-t x y' and at least 2 beacons with '-b x y'\n";
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

        cout << "Beacon " << i << ": " << beacon[i].toString() << "\t | distance: " << dist
             << "\t--> " << dist + noise << "\tnoise=" << noise << endl;
    }
    cout << "------------------------------------------------------------------\n\n";

    google::InitGoogleLogging(argv[0]);


    // The variable to solve for with its initial value. It will be
    // mutated in place by the solver.

    //TODO vedere se riesco ad usare l'oggetto point o almeno un vettore di double
    //vector<double> pos(DIM, 0.0);
//    double x = 0.0;
//    double y = 0.0;

    double x[] = {0.0, 0.0};

    // Build the problem.
    Problem problem;


    for (int i = 0; i < beacon.size(); ++i) {
        //beacon i
        vector<double> bi = {beacon[i].getX(), beacon[i].getY(), 0.0};

        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, DIM>(
                    new MyCostFunctor(bi, measures[i]));

        problem.AddResidualBlock(cost_f, NULL, x);
    }
    Solver::Options options;
    options.max_num_iterations = 25;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";
    std::cout << "Initial position: origin\n";
    std::cout << "Final position: ";
    for (int i = 0; i < DIM-1; ++i) {
        cout << x[i] << " , ";
    }
    cout << x[DIM-1] << endl;

    cout << "The estimated position is far " << target.distanceTo(Point<double>(x[0], x[1])) << " from the real position\n";


    return 0;

}
