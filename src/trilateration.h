#ifndef TRILATERATION_H
#define TRILATERATION_H

#include <iostream>
#include <random>       //gaussian random number

#include "ceres/ceres.h"

#include "point.h" // TODO meglio mettere nomi minuscoli ai file?
#include "structs.h"
#include "cost_function.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;


class Trilateration
{
public:
	const Receiver DEF_INITIAL_REC_GUESS = {Point<double>(0, 0, 0), 1e-6};

	Trilateration();
    ~Trilateration();

	Receiver computePosition(const std::vector<SatelliteMeasurement> &measurements, const double speed);

	// Only to simulate the measurements
	std::vector<SatelliteMeasurement> simulateMeasurements(const Receiver &realReceiver,
				const std::vector< Point<double> > &satellites, const double noiseStdDev,
				const double speed, bool verbose = false);

    // Setter & Getter
	Receiver getInitialReceiverGuess() const;
	void setInitialReceiverGuess(const Receiver &value);

	void setVerboseMode(bool value);

private:
	// Last known position
	Receiver initialRecGuess;
	bool verboseMode;
	std::default_random_engine generator;

};

#endif // TRILATERATION_H
