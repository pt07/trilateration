#ifndef POINT_H
#define POINT_H

#include <math.h>   //sqrt
#include <sstream>  //ostringstream


template <class T>
class Point
{
    private:
        T x;
        T y;
        T z;

    protected:

    public:
        Point();
        Point(T x, T y, T z);
        virtual ~Point();

        T getX() { return x; }
        T getY() { return y; }
        T getZ() { return z; }

        void setX(T x_) { x = x_; }
        void setY(T y_) { y = y_; }
        void setZ(T z_) { z = z_; }

        T distanceTo(Point<T> p);

        inline std::string toString() const {
            std::ostringstream s;
            s << "(" << x << ", " << y << ", " << z << ")";
            return s.str();
        }

};

#endif // POINT_H
