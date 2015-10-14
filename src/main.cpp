/*
 * ./main -r 3 -3 2 -s 15 26 40 -s 1 -32 50 -s -60 -8 70 -s 24 73 56 -s -99 -66 708
 */
#include <iostream>
#include <string.h>     //strcmp

#include "glog/logging.h"

#include "Trilateration.h"
#include "structs.h"

using namespace std;

bool parseArgs(int argc, char** argv, Receiver &realReceiver, vector<Point<double> > &satellites, double &std_dev);

// Default values
const Receiver DEF_REAL_RECEIVER = {Point<double>(), 100e-9};
const double DEF_STD_DEV = 1e-9;

const double SPEED_OF_LIGHT = 3e8; // m / s


int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);

	double std_dev = DEF_STD_DEV;
	Receiver realReceiver = DEF_REAL_RECEIVER;
	vector<Point<double>> satellites;

	if(!parseArgs(argc, argv, realReceiver, satellites, std_dev))
    {
        cout << "Input is not valid\n";
        return -1;
    }

	// Simulate measurements between target (known) and satellites
	vector<SatelliteMeasurement> measurements =
			Trilateration::simulateMeasurements(realReceiver, satellites, std_dev, SPEED_OF_LIGHT, true);


	Trilateration tr;
	tr.setVerboseMode(true);

	Receiver estReceiver = tr.computePosition(measurements, SPEED_OF_LIGHT);

	cout << "\nInitial receiver guess:\t" << tr.getInitialReceiverGuess().toString()
			<< "\t|\tnoise std dev = " << std_dev << endl << endl;

	cout << "Real receiver:\t\t" << realReceiver.toString() << endl;
	cout << "Estimated receiver:\t" << estReceiver.toString() << endl << endl;


	Point<double> diff = realReceiver.coords + -estReceiver.coords;

	cout << "Coords difference:\t" << diff.toString() << endl
		 << "Coords distance:\t" << realReceiver.coords.distanceTo(estReceiver.coords) << "\t\t\t(0=good)" << "\n";
	cout << "Bias ratio:\t\t" << realReceiver.bias/estReceiver.bias << "\t\t\t(1=good)" << endl << endl;

    return 0;

}


// Boost gives a nice args parser
bool parseArgs(int argc, char** argv, Receiver &realReceiver, vector<Point<double>> &satellites, double &std_dev)
{
	bool receiver_setted = false;
	int n_satellites = 0;

	for (int i = 1; i < argc; ++i){
		if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

			std_dev =  atof(argv[++i]);

		} else if (strcmp (argv[i], "--bias") == 0  || (strcmp (argv[i], "-b") == 0)){

			realReceiver.bias = atof(argv[++i]);

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

			realReceiver.coords = Point<double>(x, y, z);
			receiver_setted = true;
		}
	}

	// TODO check if input is well formed

	return receiver_setted && (n_satellites >= 3);
}

