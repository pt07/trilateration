#ifndef TRILATERATION_H
#define TRILATERATION_H

#include <iostream>
#include <random>       //gaussian random number

#include "ceres/ceres.h"

#include "Point.h"
#include "CostFunction.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

class Trilateration
{
public:
    const double DEF_INITIAL_BIAS_GUESS = 120e-9;
    const Point<double> DEF_INITIAL_COORDS_GUESS =  Point<double>(0, 0, 0);
    const double SPEED_OF_LIGHT = 3e8; // m / s

    Trilateration();
    ~Trilateration();

    bool computePosition(const std::vector<double> &measurements);

    // Get results
    double getEstimatedBias() const;
    Point<double> getEstimatedCoords() const;

    // Only to simulate the measurements
    std::vector<double> simulateMeasurements(const Point<double> &receiver, const double bias, const double noiseStdDev);

    // Setter & Getter
    void setSatellite(double x, double y, double z);
    void setSatellite(const Point<double> &value);
    Point<double> getSatellite(int i) const;

    void setSatellites(const std::vector< Point<double> > vec);
    std::vector< Point<double> > getSatellites() const;

    void setInitialBiasGuess(double value);
    double getInitialBiasGuess() const;

    void setInitialCoordsGuess(const Point<double> &value);
    Point<double> getInitialCoordsGuess() const;


private:
    std::vector< Point<double> > satellites;

    double initialBiasGuess;
    Point<double> initialCoordsGuess;

    // Results
    double estCoords[3];
    double estBias;
};

#endif // TRILATERATION_H
