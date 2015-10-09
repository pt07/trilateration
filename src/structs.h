#ifndef STRUCTS_H
#define STRUCTS_H

#include <sstream>  //ostringstream

struct Receiver
{
	Point<double> coords;
	double bias;

	std::string toString() const
	{
		std::ostringstream s;
		s << "[" << coords.toString() << ", " << bias << "]";
		return s.str();
	}
};


struct SatelliteMeasurement
{
	Point<double> coords;
	double pseudorange;

	std::string toString() const
	{
		std::ostringstream s;
		s << "[" << coords.toString() << ", " << pseudorange << "]";
		return s.str();
	}
};


#endif // STRUCTS_H

