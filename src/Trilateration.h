#ifndef TRILATERATION_H
#define TRILATERATION_H

//#include <iostream> // TODO forse puoi toglierla, ora è qui per il cout
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
    const double DEF_INITIAL_BIAS_GUESS = 120e-9;
    const Point<double> DEF_INITIAL_COORDS_GUESS =  Point<double>(0, 0, 0);
    const double SPEED_OF_LIGHT = 3e8; // m / s

    Trilateration();
    ~Trilateration();

    void compute(const std::vector<double> &measurements);
    void compute(const Point<double> &receiver, const double bias, const double noiseStdDev);

    // Get results
    double getEstimatedBias() const;
    Point<double> getEstimatedCoords() const;

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

    // Only to simulate the measurements
    void setBias(double value);
    double getBias() const;
    void setNoiseStdDev(double value);
    double getNoiseStdDev() const;
    std::vector<double> simulateMeasurements(const Point<double> &receiver, const double bias, const double noiseStdDev);

    std::string report();

private:
    std::vector< Point<double> > satellites;

    double initialBiasGuess; // todo const
    Point<double> initialCoordsGuess;



    // Results
    double estCoords[3];
    double estBias;

    std::ostringstream rep;

};

#endif // TRILATERATION_H
