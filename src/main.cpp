/*
 * ./main -r 2 2 2 -s -25 36 140 -s 10 -32 150 -s -60 -8 170 -s 24 55 156 -s 99 16 188
 * ./main -r 2 2 2 -s -2500 3060 14000 -s 1000 -3200 15000 -s -6000 -800 17000 -s 2400 5500 15600 -s 9900 1600 18800
 */
#include <iostream>
#include <string.h>     //strcmp

#include "glog/logging.h"

#include "trilateration.h"
#include "structs.h"

using namespace std;

bool parseArgs(int argc, char** argv, Receiver &realReceiver, vector<Point<double> > &satellites, double &std_dev);

// Default values
const Receiver DEF_REAL_RECEIVER = {Point<double>(), 100e-9};
const double DEF_STD_DEV = 1e-10;

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

	Trilateration tr;
	tr.setVerboseMode(true);

	// Simulate measurements between target (known) and satellites
	vector<SatelliteMeasurement> measurements = tr.simulateMeasurements(realReceiver,
											satellites, std_dev, SPEED_OF_LIGHT, true);

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

