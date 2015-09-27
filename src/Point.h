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
        std::vector<T> coords;

    protected:

    public:
        Point();
        Point(T x, T y, T z);
        virtual ~Point();

        inline T getX() { return coords[0]; }
        inline T getY() { return coords[1]; }
        inline T getZ() { return coords[2]; }

        inline void setCoords(T x, T y, T z){ coords = {x, y, z}; }
        inline std::vector<T> getCoords() { return coords; }

        inline void setX(T x) { coords[0] = x; }
        inline void setY(T y) { coords[1] = y; }
        inline void setZ(T z) { coords[2] = z; }

        T distanceTo(Point<T> p);

        inline std::string toString() const {
            std::ostringstream s;
            s << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")";
            return s.str();
        }

};

#endif // POINT_H
