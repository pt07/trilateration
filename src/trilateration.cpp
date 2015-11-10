#include "trilateration.h"

Trilateration::Trilateration()
	: initialRecGuess(DEF_INITIAL_REC_GUESS),
	  verboseMode(false),
	  generator(time(NULL))
{ }

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
void Trilateration::simulateMeasurements(const Receiver &realReceiver,
					std::vector<SatelliteMeasurement> &measurements,
					const double noiseStdDev, const double speed, bool verbose)
{
	std::normal_distribution<double> distribution(0, noiseStdDev);

	if(verbose)
		std::cout << "Simulated measurements:\n";

	for (size_t i=0; i<measurements.size(); ++i){
        double noise = distribution(generator);

		measurements[i].pseudorange = realReceiver.pos.distanceTo(measurements[i].pos)
											+ speed * (realReceiver.bias + noise);

		if(verbose)
			std::cout << "--Measure " << i << ": " <<  measurements[i].toString()
					<< "\t // = distance (" << realReceiver.pos.distanceTo(measurements.at(i).pos)
					<< ") + bias*speed (" << realReceiver.bias * speed
					<< ") + noise*speed (" << noise * speed << ")\n";

    }
	if(verbose)
		std::cout << "\n";

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
