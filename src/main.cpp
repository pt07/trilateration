/*
 *
 *
 */
#include <iostream>
#include <string.h>     //strcmp

#include "Trilateration.h"

using namespace std;

bool parseArgs(int argc, char** argv, Trilateration &tr, double &bias, double &std_dev);

// Default values
const double DEF_BIAS = 100e-9;
const double DEF_STD_DEV = 1e-9;


int main(int argc, char** argv)
{

    Trilateration *tr = new Trilateration();
    double bias = DEF_BIAS,
           std_dev = DEF_STD_DEV;

    if(!parseArgs(argc, argv, *tr, bias, std_dev))
    {
        cout << "Input is not valid\n";
        return -1;
    }

    cout << "satelliti " << tr->getSatellites().size() << endl;

    Point<double> rec = tr->getSatellite(0);

    cout << "receiver " << rec.toString() << endl;

    // the class will simulate measurements based on this bias and noise
    Point<double> myPosTODO;
    double myBiasTODO;

    tr->compute(bias, std_dev, myPosTODO, myBiasTODO);
    cout << "La posizione calcolata è " << myPosTODO.toString() << " e bias " << myBiasTODO << endl;




    return 0;

}



bool parseArgs(int argc, char** argv, Trilateration &tr, double &bias, double &std_dev)
{
    bool valid_input = true;

    for (int i = 1; i < argc; ++i){
        if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

            std_dev =  atof(argv[++i]);

        } else if (strcmp (argv[i], "--bias") == 0  || (strcmp (argv[i], "-b") == 0)){

            bias = atof(argv[++i]);

        } else if ((strcmp (argv[i], "--satellite") == 0) || (strcmp (argv[i], "-s") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = atof(argv[++i]);

            tr.setSatellite(x, y, z);

        } else if ((strcmp (argv[i], "--receiver") == 0) || (strcmp (argv[i], "-r") == 0)){

            double x = atof(argv[++i]);
            double y = atof(argv[++i]);
            double z = atof(argv[++i]);

            tr.setReceiver(x, y, z);
        }
    }

    // TODO check if input is well formed

    return valid_input;
}
