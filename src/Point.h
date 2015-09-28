#ifndef POINT_H
#define POINT_H

#include <math.h>   //sqrt
#include <sstream>  //ostringstream
#include <vector>

template <class T>
class Point
{
    private:
        std::vector<T> coords;

    public:
        Point();
        Point(T x, T y, T z);
        virtual ~Point();

        inline std::vector<T> getCoords() { return coords; }
        inline T getX() { return coords[0]; }
        inline T getY() { return coords[1]; }
        inline T getZ() { return coords[2]; }

        inline void setCoords(T x, T y, T z){ coords = {x, y, z}; }
        inline void setX(T x) { coords[0] = x; }
        inline void setY(T y) { coords[1] = y; }
        inline void setZ(T z) { coords[2] = z; }

        T distanceTo(Point<T> p);

        std::string toString() const;

};

#endif // POINT_H
