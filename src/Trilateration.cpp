#include "Trilateration.h"

Trilateration::Trilateration()
    : receiver(Point<double>()),
      initialBiasGuess(DEF_INITIAL_BIAS_GUESS),
      initialCoordsGuess(DEF_INITIAL_COORDS_GUESS)
{
    std::cout << "costruttore!\n"; // std::cin.ignore();
}

Trilateration::~Trilateration()
{
    //TODO is needed?
    std::cout << "Destructor\n";
}

std::vector<double> Trilateration::simulateMeasurements(double bias, double noiseStdDev)
{
    std::default_random_engine generator(time(NULL));
    std::normal_distribution<double> distribution(0, noiseStdDev);

    std::vector<double> v;

    for (size_t i=0; i<satellites.size(); ++i){
        double time = receiver.distanceTo(satellites[i]) / SPEED_OF_LIGHT;
        double noise = distribution(generator);

        v.push_back(time + bias + noise);
    }

    return v;
}



void Trilateration::compute(const std::vector<double> &measurements, Point<double> &est_pos, double &est_bias )
{
    std::cout << "CALCOLO\n";
    est_pos = Point<double>(7, 8, 9);
    est_bias = 1e-5;
    return;
}

void Trilateration::compute(double bias, double noiseStdDev, Point<double> &est_pos, double &est_bias )
{
    std::cout << "Using simulated measurements!\n";
    std::vector<double> measurements = simulateMeasurements(bias, noiseStdDev);
    compute(measurements, est_pos, est_bias);
}

/*
 * Setter
 */
void Trilateration::setReceiver(double x, double y, double z)
{
    setReceiver(Point<double>(x, y, z));
}

void Trilateration::setReceiver(const Point<double> &value)
{
    receiver = value;
}

void Trilateration::setSatellite(double x, double y, double z)
{
    satellites.push_back(Point<double>(x, y, z));
}

void Trilateration::setSatellite(const Point<double> &value)
{
    satellites.push_back(value);
}

void Trilateration::setInitialBiasGuess(double value)
{
    initialBiasGuess = value;
}

// Receive time measurements from the sensor
void Trilateration::setTimeMeasurements(const std::vector<double> &value)
{
    timeMeasurements = value;
}



/*
 * Getter
 */
Point<double> Trilateration::getReceiver() const
{
    return receiver;
}

Point<double> Trilateration::getSatellite(int i) const
{
    //TODO decidi cosa tornare se index out of bound
    return satellites.at(i); //TODO differenza tra at e []?
}

std::vector< Point<double> > Trilateration::getSatellites() const
{
    return satellites;
}

Point<double> Trilateration::getInitialCoordsGuess() const
{
    return initialCoordsGuess;
}

double Trilateration::getInitialBiasGuess() const
{
    return initialBiasGuess;
}

void Trilateration::setInitialCoordsGuess(const Point<double> &value)
{
    initialCoordsGuess = value;
}

std::vector<double> Trilateration::getTimeMeasurements() const
{
    return timeMeasurements;
}
