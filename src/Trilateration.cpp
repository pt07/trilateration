#include "Trilateration.h"

Trilateration::Trilateration(bool verb)
	: initialRecGuess(DEF_INITIAL_REC_GUESS), verboseMode(verb) { }

Trilateration::~Trilateration() { }

Receiver Trilateration::computePosition(const std::vector<SatelliteMeasurement> &sm, const double speed)
{
	double estCoords[3];
	double estBias;

	// Build the problem.
	Problem problem;
	for (size_t i = 0; i < sm.size(); ++i) {
        CostFunction* cost_f = new AutoDiffCostFunction<MyCostFunctor, 1, 3, 1>(
					new MyCostFunctor(sm.at(i), speed));

        problem.AddResidualBlock(cost_f, NULL, estCoords, &estBias);
    }

    Solver::Options options;
	options.minimizer_progress_to_stdout = verboseMode;
    options.minimizer_type = ceres::TRUST_REGION;
    //options.minimizer_type = ceres::LINE_SEARCH; // TODO http://ceres-solver.org/nnls_solving.html#Solver::Options::linear_solver_type__LinearSolverType
    //options.linear_solver_type = ceres::DENSE_QR;

    options.function_tolerance = 1e-12;
    options.gradient_tolerance = 1e-12;
    options.parameter_tolerance = 1e-12;
    options.max_num_iterations = 1000;

    Solver::Summary summary;
    Solve(options, &problem, &summary);

	if(verboseMode) std::cout << summary.BriefReport() << "\n";

	return {Point<double>(estCoords[0], estCoords[1], estCoords[2]), estBias};
}

//static function
std::vector<SatelliteMeasurement> Trilateration::simulateMeasurements(const Receiver &realReceiver,
		const std::vector<Point<double> > &satellites, const double noiseStdDev, const double speed)
{
	// Activate random
    std::default_random_engine generator(time(NULL));
	std::normal_distribution<double> distribution(0, noiseStdDev);

	std::vector<SatelliteMeasurement> v;

    std::cout << "Simulated measurements:\n";
	for (size_t i=0; i<satellites.size(); ++i){
		double time = realReceiver.coords.distanceTo(satellites.at(i)) / speed;
        double noise = distribution(generator);

		SatelliteMeasurement meas;
		meas.coords = satellites.at(i);
		meas.pseudorange = time + realReceiver.bias + noise;

		std::cout << "--Measure " << i << ": " <<  meas.toString()
			<< "\t // = time (" << time
			<< ") + bias (" << realReceiver.bias
			<< ") + noise (" << noise << ")\n";

		v.push_back(meas);
    }
    std::cout << "\n";

    return v;
}


/*
 * Setter & Getter
 */

Receiver Trilateration::getInitialReceiverGuess() const
{
	return initialRecGuess;
}

void Trilateration::setInitialReceiverGuess(const Receiver &value)
{
	initialRecGuess = value;
}

void Trilateration::setVerboseMode(bool value)
{
	verboseMode = value;
}
