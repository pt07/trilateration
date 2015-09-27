#include "Point.h"

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

