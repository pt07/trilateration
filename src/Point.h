#ifndef POINT_H
#define POINT_H

#include <math.h> //sqrt

//TODO Point a piu dimensioni

template <class T> class Point
{
    private:
        T x;
        T y;

    protected:

    public:
        Point();
        Point(T x, T y);
//        Point(const Point<T>& p);
        virtual ~Point();

        T getX() { return x; }
        T getY() { return y; }

        void setX(T x_) { x = x_; }
        void setY(T y_) { y = y_; }

        T distanceTo(Point<T> p);

};


#endif // POINT_H
