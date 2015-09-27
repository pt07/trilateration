#ifndef POINT_H
#define POINT_H

#include <math.h>   //sqrt
#include <sstream>  //ostringstream
#include <vector>

//TODO usare vector<T> e rifare getter

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

        inline T getX() { return x; }
        inline T getY() { return y; }
        inline T getZ() { return z; }
        inline std::vector<T> getXYZ() { return {x, y, z}; }

        inline void setX(T x_) { x = x_; }
        inline void setY(T y_) { y = y_; }
        inline void setZ(T z_) { z = z_; }

        T distanceTo(Point<T> p);

        inline std::string toString() const {
            std::ostringstream s;
            s << "(" << x << ", " << y << ", " << z << ")";
            return s.str();
        }

};

#endif // POINT_H
