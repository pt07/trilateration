#ifndef TRILATERATION_H
#define TRILATERATION_H

#include <iostream>
#include <random>       //gaussian random number

#include "ceres/ceres.h"

#include "Point.h" // TODO meglio mettere nomi minuscoli ai file?
#include "structs.h"
#include "CostFunction.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;


class Trilateration
{
public:
	const Receiver DEF_INITIAL_REC_GUESS = {Point<double>(0, 0, 0), 120e-9};

	Trilateration(bool verb = true);
    ~Trilateration();

	Receiver computePosition(const std::vector<SatelliteMeasurement> &measurements, const double speed);

	// Only to simulate the measurements
	static std::vector<SatelliteMeasurement> simulateMeasurements(const Receiver &realReceiver,
				const std::vector< Point<double> > &satellites, const double noiseStdDev, const double speed);

    // Setter & Getter
	Receiver getInitialReceiverGuess() const;
	void setInitialReceiverGuess(const Receiver &value);

	void setVerboseMode(bool value);

private:
	// Last known position
	Receiver initialRecGuess;
	bool verboseMode;

};

#endif // TRILATERATION_H
