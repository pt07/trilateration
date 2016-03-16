/*
 * ./main_pr -s x_sat y_sat z_sat pr_sat -s.....
 */
#include <iostream>
#include <string.h>     //strcmp

#include "glog/logging.h"

#include "trilateration.h"
#include "structs.h"

// to read file
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp> //to tokenize strings
#include <iomanip> // for using setprecision on cout

using namespace std;

bool parseArgs(int argc, char** argv, Receiver &realReceiver,
					vector<SatelliteMeasurement> &measurements, double &std_dev);

// Default values
//const Receiver DEF_REAL_RECEIVER = {Point<double>(0, 0, 0), 100e-9};
const double DEF_STD_DEV = 1e-10;


int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);

	double std_dev = DEF_STD_DEV;

	Receiver realReceiver = {Point<double>(0, 0, 0), 100e-9};

	vector<SatelliteMeasurement> measurements;

	Trilateration tr;
	tr.setVerboseLevel(2);

	if(!parseArgs(argc, argv, realReceiver, measurements, std_dev))
    {
        cout << "Input is not valid\n";
        return -1;
    }


	for (size_t i = 0; i < measurements.size(); ++i) {
		cout << "Measure " << i << ": " << measurements[i].toString() << endl;
	}
	cout << endl;


	Receiver estReceiver = tr.computePosition(measurements);

	cout << "\nInitial receiver guess:\t" << tr.getInitialReceiverGuess().toString()
			<< "\t|\tnoise std dev = " << std_dev << endl << endl;

	cout << "Real receiver:\t\t" << realReceiver.toString() << endl;
	cout << "Estimated receiver:\t" << estReceiver.toString() << endl << endl;


	Point<double> diff = realReceiver.pos + -estReceiver.pos;

	cout << "Coords difference:\t" << diff.toString() << endl
		 << "Coords distance:\t" << realReceiver.pos.distanceTo(estReceiver.pos) << "\t\t\t(0=good)" << "\n";
	cout << "Bias ratio:\t\t" << realReceiver.bias/estReceiver.bias << "\t\t\t(1=good)" << endl << endl;

    return 0;

}


// Boost gives a nice args parser
bool parseArgs(int argc, char** argv, Receiver &realReceiver,
			   vector<SatelliteMeasurement> &measurements, double &std_dev)
{
	string path;

	for (int i = 1; i < argc; ++i){
		if ((strcmp (argv[i], "--dev") == 0) || (strcmp (argv[i], "-d") == 0)){

			std_dev =  atof(argv[++i]);

		} else if (strcmp (argv[i], "--bias") == 0  || (strcmp (argv[i], "-b") == 0)){

			realReceiver.bias = atof(argv[++i]);

		} else if ((strcmp (argv[i], "--satellite") == 0) || (strcmp (argv[i], "-s") == 0)){

			double x = atof(argv[++i]);
			double y = atof(argv[++i]);
			double z = atof(argv[++i]);
			double pr = atof(argv[++i]);

			SatelliteMeasurement sm;
			sm.pos = Point<double>(x, y, z);
			sm.pseudorange = pr;

			measurements.push_back(sm);

		}
	}


	// TODO check if input is well formed
	return measurements.size() >= 3;
}

