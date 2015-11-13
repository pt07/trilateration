#ifndef STRUCTS_H
#define STRUCTS_H

#include <sstream>  //ostringstream
#include <iomanip>	//set precision
#include "point.h"


struct Receiver
{
	Point<double> pos;
	double bias;

	std::string toString() const
	{
		std::ostringstream s;
		s << std::setprecision(12);
		s << "[" << pos.toString() << ", " << bias << "]";
		return s.str();
	}
};


struct SatelliteMeasurement
{
	Point<double> pos;
	double pseudorange;

	std::string toString() const
	{
		std::ostringstream s;
		s << std::setprecision(12);
		s << "[" << pos.toString() << ", " << pseudorange << "]";
		return s.str();
	}
};


#endif // STRUCTS_H

