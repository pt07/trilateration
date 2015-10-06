#include "Trilateration.h"

Trilateration::Trilateration()
{
    std::cout << "Constructor!\n";

    initialBiasGuess = DEF_INITIAL_BIAS_GUESS;
    initialCoordsGuess = DEF_INITIAL_COORDS_GUESS;

}

Trilateration::~Trilateration()
{
    //TODO is needed?
    std::cout << "Destructor\n";
}

void Trilateration::compute(const std::vector<double> &measurements)
{
    std::cout << "----- INIZIO CALCOLO ------\n";

    // Build the problem.
    Problem problem;


    for (size_t i = 0; i < satellites.size(); ++i) {
        std::vector<double> sat_i = satellites[i].getCoords();
        std::cout << "sat_i  = (" << sat_i[0] << " " << sat_i[1] << " " << sat_i[2] <<  ")\t";
        std::cout << "measurements[i] " << measurements[i] << "\n";



        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3, 1>(
                    new MyCostFunctor(sat_i, measurements[i], SPEED_OF_LIGHT));

        problem.AddResidualBlock(cost_f, NULL, estCoords, &estBias);
    }


    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    options.parameter_tolerance = 1e-12;
    options.function_tolerance = 1e-12;
    options.gradient_tolerance = 1e-12;
    options.max_num_iterations = 1000;

    Solver::Summary summary;
    Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";

    std::cout << "---------  FINE  ----------\n\n";
    return;
}

void Trilateration::compute(const Point<double> &receiver, const double bias, const double noiseStdDev)
{
    std::cout << "Using simulated measurements!\n";
    std::vector<double> measurements = simulateMeasurements(receiver, bias, noiseStdDev);
    compute(measurements);
}

std::vector<double> Trilateration::simulateMeasurements(const Point<double> &receiver, const double bias, const double noiseStdDev)
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




/*
 * TODO stampa riassuntino classe
 */
std::string Trilateration::report()
{
    return rep.str();
}



/*
 * Setter & Getter
 */
double Trilateration::getEstimatedBias() const
{
    return estBias;
}

Point<double> Trilateration::getEstimatedCoords() const
{
    return Point<double>(estCoords[0], estCoords[1], estCoords[2]);
}

void Trilateration::setSatellite(double x, double y, double z)
{
    satellites.push_back(Point<double>(x, y, z));
}

void Trilateration::setSatellite(const Point<double> &value)
{
    satellites.push_back(value);
}

void Trilateration::setSatellites(const std::vector<Point<double> > vec)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        satellites.push_back(vec[i]);
    }
}

void Trilateration::setInitialBiasGuess(double value)
{
    initialBiasGuess = value;
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
