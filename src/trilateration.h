#ifndef TRILATERATION_H
#define TRILATERATION_H

#include <iostream>
#include <random>       //gaussian random number

#include "ceres/ceres.h"

#include "point.h"
#include "structs.h"
#include "cost_function.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

//TODO add a namespace?

class Trilateration
{
public:
	const Receiver DEF_INITIAL_REC_GUESS = {Point<double>(0, 0, 0), 1e-6};
	static const int SPEED_OF_LIGHT = 299792458;// [m/s]

	Trilateration();
    ~Trilateration();

	Receiver computePosition(const std::vector<SatelliteMeasurement> &measurements, const double speed = SPEED_OF_LIGHT);

	// Only to simulate the measurements
	void simulateMeasurements(const Receiver &realReceiver,
				std::vector<SatelliteMeasurement> &satellites, const double noiseStdDev,
				bool verbose = false, const double speed = SPEED_OF_LIGHT);

    // Setter & Getter
	Receiver getInitialReceiverGuess() const;
	void setInitialReceiverGuess(const Receiver &value);

	void setVerboseLevel(int value);

private:
	// Last known position
	Receiver initialRecGuess;
	int verboseLevel;
	std::default_random_engine generator;

};

#endif // TRILATERATION_H
