#ifndef COSTFUNCTION_H
#define COSTFUNCTION_H


class MyCostFunctor{

public:
    MyCostFunctor(std::vector<double> sat_i_, double m_i_, double vel_)
        : sat_i(sat_i_), m_i(m_i_), vel(vel_) {}

    template <typename T>
    bool operator()(const T* const pos, const T* const bias, T* residual) const {

        T square_sum = T(0);
        for (int i = 0; i < sat_i.size(); ++i) {
            square_sum += pow(pos[i] - T(sat_i[i]) , 2);
        }
        T distance = (square_sum != T(0)) ? sqrt(square_sum) : T(0) ;

        /*
         * error = expected measurement - actual measurement
         * con  expected measurement = distance / vel + bias[0]
         *      actual measurement   = m_i
         */
        residual[0] = distance / vel + bias[0] - m_i;

        return true;
    }


private:
    const std::vector<double> sat_i;
    const double m_i;
    const double vel;
};


#endif // COSTFUNCTION_H
