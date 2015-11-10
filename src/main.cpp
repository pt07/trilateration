/*
 * ./main -r 2 2 2 -s -25 36 140 -s 10 -32 150 -s -60 -8 170 -s 24 55 156 -s 99 16 188
 * ./main -r 2 2 2 -s -2500 3060 14000 -s 1000 -3200 15000 -s -6000 -800 17000 -s 2400 5500 15600 -s 9900 1600 18800
 * ./main -f ../../gpstk_test/build/log_epoch.txt
 */
#include <iostream>
#include <string.h>     //strcmp

#include "glog/logging.h"

#include "Trilateration.h"
#include "structs.h"

// to read file
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp> //to tokenize strings
#include <iomanip> // for using setprecision on cout

using namespace std;

bool parseArgs(int argc, char** argv, Trilateration &tr, Receiver &realReceiver,
					vector<SatelliteMeasurement> &measurements, double &std_dev);

// Default values
const Receiver DEF_REAL_RECEIVER = {Point<double>(), 100e-9};
const double DEF_STD_DEV = 1e-10;

const double SPEED_OF_LIGHT = 3e8; // m / s


int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);

	double std_dev = DEF_STD_DEV;
	Receiver realReceiver = DEF_REAL_RECEIVER;

	vector<SatelliteMeasurement> measurements;

	Trilateration tr;
	tr.setVerboseMode(true);

	if(!parseArgs(argc, argv, tr, realReceiver, measurements, std_dev))
    {
        cout << "Input is not valid\n";
        return -1;
    }


	for (size_t i = 0; i < measurements.size(); ++i) {
		cout << "Measure " << i << ": " << measurements[i].toString() << endl;
	}
	cout << endl;


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
bool parseArgs(int argc, char** argv, Trilateration &tr, Receiver &realReceiver,
			   vector<SatelliteMeasurement> &measurements, double &std_dev)
{
	bool receiver_setted = false;
	bool readFromFile = false;
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

			SatelliteMeasurement sm;
			sm.coords = Point<double>(x, y, z);
			sm.pseudorange = 0.0;

			measurements.push_back(sm);

		} else if ((strcmp (argv[i], "--receiver") == 0) || (strcmp (argv[i], "-r") == 0)){

			double x = atof(argv[++i]);
			double y = atof(argv[++i]);
			double z = atof(argv[++i]);

			realReceiver.coords = Point<double>(x, y, z);
			receiver_setted = true;
		} else if ((strcmp (argv[i], "--file") == 0) || (strcmp (argv[i], "-f") == 0)){

			//segnati il nome file e vai a leggere
			readFromFile = true;
			path = argv[++i];
			break;

		}
	}



	//leggi da file
	if(readFromFile){

		string line;
		ifstream logfile (path);

		if (logfile.is_open())
		{
			//receiver is on the first row
			getline (logfile, line);
			vector<string> strs;
			boost::split(strs, line, boost::is_any_of(" "));

			realReceiver.coords = Point<double>(atof(strs[0].c_str()), atof(strs[1].c_str()), atof(strs[2].c_str()));
			realReceiver.bias = atof(strs[3].c_str());

			cout << "READING FROM FILE " << path << endl;
			cout << "Receiver: " << realReceiver.toString() << endl;

			receiver_setted = true;

			//read all the satellites
			measurements.clear();

			while ( getline (logfile, line) )
			{
				boost::split(strs, line, boost::is_any_of(" "));

				SatelliteMeasurement sm;
				sm.coords = Point<double>(atof(strs[0].c_str()), atof(strs[1].c_str()), atof(strs[2].c_str()));
				sm.pseudorange = atof(strs[3].c_str());

				measurements.push_back(sm);
			}
			logfile.close();
		}

	} else {

		// Simulate measurements between target (known) and satellites
		tr.simulateMeasurements(realReceiver, measurements, std_dev, SPEED_OF_LIGHT, true);
	}


	// TODO check if input is well formed
	return receiver_setted && (measurements.size() >= 3);
}

