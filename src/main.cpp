/*
 *
 *
 */
#include <iostream>
#include <string.h>     //strcmp

#include "glog/logging.h"

#include "Trilateration.cpp" // TODO toask: the same problem

using namespace std;

bool parseArgs(int argc, char** argv, Point<double> &receiver, vector<Point<double> > &satellites, double &bias, double &std_dev);

// Default values
const double DEF_BIAS = 100e-9;
const double DEF_STD_DEV = 1e-9;


int main(int argc, char** argv)
{

    Trilateration *tr = new Trilateration();

    double bias = DEF_BIAS,
           std_dev = DEF_STD_DEV;
    Point<double> receiver;
    vector<Point<double>> satellites;

    if(!parseArgs(argc, argv, receiver, satellites, bias, std_dev))
    {
        cout << "Input is not valid\n";
        return -1;
    }

    tr->setSatellites(satellites);

    google::InitGoogleLogging(argv[0]);

    // The class will simulate measurements based on this receiver position, bias and noise.
    // If you want to use real measurements, call tr->compute(vector<double> &measurements);
    tr->compute(receiver, bias, std_dev);

    Point<double> receiver_est = tr->getEstimatedCoords();
    double est_bias = tr->getEstimatedBias();

    cout << "Initial guess: position" << tr->getInitialCoordsGuess().toString() << "\tbias " << tr->getInitialBiasGuess() << endl;

    cout << "BIAS:\t  real = " << bias
         << "\n\t  estimated = " << est_bias
         << "\n\t  ratio = " << est_bias/bias << endl;

    cout << "POSITION: real: " << receiver.toString()
         << "\n\t  estimated: " << receiver_est.toString()
         << "\n\t  distance: " << receiver.distanceTo(receiver_est) << "\n\n";



    return 0;

}



bool parseArgs(int argc, char** argv, Point<double> &receiver, vector<Point<double>> &satellites, double &bias, double &std_dev)
{
    bool receiver_setted = false;
    int n_satellites = 0;

    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev =  atof(argv[++i]);

        } else if (strcmp (argv[i], "--bias") == 0  || (strcmp (argv[i], "-b") == 0)){

            bias = atof(argv[++i]);

        } else if ((strcmp (argv[i], "--satellite") == 0) || (strcmp (argv[i], "-s") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = atof(argv[++i]);

            satellites.push_back(Point<double>(x, y, z));
            ++n_satellites;

        } else if ((strcmp (argv[i], "--receiver") == 0) || (strcmp (argv[i], "-r") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = atof(argv[++i]);

            receiver = Point<double>(x, y, z);
            receiver_setted = true;
        }
    }

    // TODO check if input is well formed

    return receiver_setted && (n_satellites >= 3);
}
