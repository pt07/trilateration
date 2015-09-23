#include "Point.h"

template <class T> Point<T>::Point() : x(0), y(0) {}

template <class T> Point<T>::Point(T x, T y) : x(x), y(y) {}

template <class T> Point<T>::~Point() {}

template <class T> T Point<T>::distanceTo(Point<T> p){

    T sum = (x - p.getX()) * (x - p.getX()) + (y - p.getY()) * (y - p.getY());

    return sqrt(sum);
}

