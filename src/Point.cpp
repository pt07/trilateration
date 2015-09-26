#include "Point.h"

template <class T, int n>
Point<T, n>::Point() : x(0), y(0) {}

template <class T, int n>
Point<T, n>::Point(T x, T y) : x(x), y(y) {}

template <class T, int n>
Point<T, n>::~Point() {}

template <class T, int n>
T Point<T, n>::distanceTo(Point<T, n> p){

    T sum = pow(x - p.getX(), 2) + pow(y - p.getY(), 2);

    return sqrt(sum);
}

