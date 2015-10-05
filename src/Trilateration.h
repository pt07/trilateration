#ifndef TRILATERATION_H
#define TRILATERATION_H

#include <iostream> // TODO forse puoi toglierla, ora è qui per il cout
#include <random>       //gaussian random number

#include "Point.h"


class Trilateration
{
public:
    Trilateration();
    ~Trilateration();

    double getTimeDistance(int nSat);

    void compute(const std::vector<double> &measurements, Point<double> &est_pos, double &est_bias );
    void compute(double bias, double noiseStdDev, Point<double> &est_pos, double &est_bias );

    // Setter
    void setReceiver(double x, double y, double z);
    void setReceiver(const Point<double> &value);
    void setSatellite(double x, double y, double z);
    void setSatellite(const Point<double> &value);
    void setInitialBiasGuess(double value);
    void setInitialCoordsGuess(const Point<double> &value);
    void setTimeMeasurements(const std::vector<double> &value);


    // Getter
    Point<double> getReceiver() const;
    Point<double> getSatellite(int i) const;
    std::vector< Point<double> > getSatellites() const;
    double getInitialBiasGuess() const;
    Point<double> getInitialCoordsGuess() const;
    std::vector<double> getTimeMeasurements() const;

    // Only to simulate the measurements
    std::vector<double> simulateMeasurements(double bias, double noiseStdDev);
    void setBias(double value);
    void setNoiseStdDev(double value);
    double getBias() const;
    double getNoiseStdDev() const;


private:
    Point<double> receiver;
    std::vector< Point<double> > satellites;

    double initialBiasGuess;
    Point<double> initialCoordsGuess;


    std::vector<double> timeMeasurements;




    const double DEF_INITIAL_BIAS_GUESS = 120e-9;
    const Point<double> DEF_INITIAL_COORDS_GUESS =  Point<double>(0, 0, 0);
    const double SPEED_OF_LIGHT = 3e8; // m / s

};

#endif // TRILATERATION_H
