#include "Point.h"

template <class T>
Point<T>::Point() : x(0), y(0) {}

template <class T>
Point<T>::Point(T x, T y) : x(x), y(y) {}

template <class T>
Point<T>::~Point() {}

template <class T>
T Point<T>::distanceTo(Point<T> p){

    T sum = pow(x - p.getX(), 2) + pow(y - p.getY(), 2);

    return sqrt(sum);
}

