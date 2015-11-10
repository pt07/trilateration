#ifndef COSTFUNCTION_H
#define COSTFUNCTION_H

#include "structs.h"

class MyCostFunctor{

public:

	MyCostFunctor(const SatelliteMeasurement sm_, const double speed_)
		: sm(sm_), speed(speed_) {}

    template <typename T>
    bool operator()(const T* const pos, const T* const bias, T* residual) const {

        T square_sum = T(0);
		for (int i = 0; i < sm.coords.getCoords().size(); ++i) {
			square_sum += pow(pos[i] - T(sm.coords.getCoords()[i]) , 2);
        }
        T distance = (square_sum != T(0)) ? sqrt(square_sum) : T(0) ;

		//    error = (expected measurement)     - (actual measurement)
		residual[0] = (distance + bias[0]*speed) - (sm.pseudorange);

        return true;
    }


private:
	SatelliteMeasurement sm;
    const double speed;
};


#endif // COSTFUNCTION_H
