#include "Trilateration.h"

Trilateration::Trilateration()
{
    initialBiasGuess = DEF_INITIAL_BIAS_GUESS;
    initialCoordsGuess = DEF_INITIAL_COORDS_GUESS;

}

Trilateration::~Trilateration() { }

bool Trilateration::computePosition(const std::vector<double> &measurements, const double speed)
{
    if((satellites.size() < 2) || (satellites.size() != measurements.size()))
    {
        std::cout << "Insert the same number of satellites and measurements, and at least 2!\n";
        return false;
    }

    // Build the problem.
    Problem problem;


    for (size_t i = 0; i < satellites.size(); ++i) {
        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3, 1>(
                    new MyCostFunctor(satellites.at(i).getCoords(), measurements.at(i), speed));

        problem.AddResidualBlock(cost_f, NULL, estCoords, &estBias);
    }


    Solver::Options options;
    options.minimizer_progress_to_stdout = true;

    options.minimizer_type = ceres::TRUST_REGION;
    //options.minimizer_type = ceres::LINE_SEARCH; // TODO http://ceres-solver.org/nnls_solving.html#Solver::Options::linear_solver_type__LinearSolverType
    //options.linear_solver_type = ceres::DENSE_QR;

    options.function_tolerance = 1e-12;
    options.gradient_tolerance = 1e-12;
    options.parameter_tolerance = 1e-12;
    options.max_num_iterations = 1000;




    Solver::Summary summary;
    Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";

    return true;
}

std::vector<double> Trilateration::simulateMeasurements(const Point<double> &receiver, const double bias, const double noiseStdDev, const double speed)
{
    std::default_random_engine generator(time(NULL));
    std::normal_distribution<double> distribution(0, noiseStdDev);

    std::vector<double> v;

    std::cout << "Simulated measurements:\n";
    for (size_t i=0; i<satellites.size(); ++i){
        double time = receiver.distanceTo(satellites.at(i)) / speed;
        double noise = distribution(generator);

        v.push_back(time + bias + noise);

        std::cout << "--Satellite " << i << ": " << satellites.at(i).toString()
            << "\t | time (" << time
            << ") + bias (" << bias
            << ") + noise (" << noise
            << ")\t= " << time + bias + noise << " ns" << std::endl;
    }
    std::cout << "\n";

    return v;
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
        satellites.push_back(vec.at(i));
    }
}

void Trilateration::setInitialBiasGuess(double value)
{
    initialBiasGuess = value;
}

Point<double> Trilateration::getSatellite(int i) const
{
    return satellites.at(i);
}

void Trilateration::deleteSatellite(int i)
{
    satellites.erase( satellites.begin() + i);
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
