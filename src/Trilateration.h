#ifndef TRILATERATION_H
#define TRILATERATION_H

#include <iostream> // TODO forse puoi toglierla, ora è qui per il cout
#include <random>       //gaussian random number

#include "ceres/ceres.h"

#include "Point.h"
#include "CostFunction.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

//using ceres::Covariance;

class Trilateration
{
public:
    Trilateration();
    ~Trilateration();

    double getTimeDistance(int nSat);

    void compute(const std::vector<double> &measurements);
    void compute(const Point<double> &receiver, double bias, double noiseStdDev);

    // Setter
    void setSatellite(double x, double y, double z);
    void setSatellite(const Point<double> &value);
    void setInitialBiasGuess(double value);
    void setInitialCoordsGuess(const Point<double> &value);


    // Getter
    Point<double> getSatellite(int i) const;
    std::vector< Point<double> > getSatellites() const;
    double getInitialBiasGuess() const;
    Point<double> getInitialCoordsGuess() const;

    // Only to simulate the measurements
    std::vector<double> simulateMeasurements(const Point<double> &receiver, double bias, double noiseStdDev);
    void setBias(double value);
    void setNoiseStdDev(double value);
    double getBias() const;
    double getNoiseStdDev() const;


    double getEstimatedBias() const;
    Point<double> getEstimatedCoords() const;

private:
    std::vector< Point<double> > satellites;

    double initialBiasGuess;
    Point<double> initialCoordsGuess;

    double estCoords[3];
    double estBias;


    const double DEF_INITIAL_BIAS_GUESS = 120e-9;
    const Point<double> DEF_INITIAL_COORDS_GUESS =  Point<double>(0, 0, 0);
    const double SPEED_OF_LIGHT = 3e8; // m / s

};

#endif // TRILATERATION_H
