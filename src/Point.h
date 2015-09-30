#ifndef POINT_H
#define POINT_H

#include <math.h>   //sqrt
#include <sstream>  //ostringstream
#include <vector>

//class header
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


// class implementation

template <class T>
Point<T>::Point()
    : Point(T(0), T(0), T(0)){}

template <class T>
Point<T>::Point(T x, T y, T z = T(0))
    : coords({x, y, z}) {}

template <class T>
Point<T>::~Point() {}

template <class T>
T Point<T>::distanceTo(Point<T> p){

    T sum =   pow(coords[0] - p.getX(), 2)
            + pow(coords[1] - p.getY(), 2)
            + pow(coords[2] - p.getZ(), 2);

    return sqrt(sum);
}

template <class T>
std::string Point<T>::toString() const {
    std::ostringstream s;
    s << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")";
    return s.str();
}


#endif // POINT_H
