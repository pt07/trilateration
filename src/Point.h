#ifndef POINT_H
#define POINT_H

#include <math.h>   //sqrt
#include <sstream>  //ostringstream

//TODO Point a piu dimensioni

template <class T>
class Point
{
    private:
        T x;
        T y;

    protected:

    public:
        Point();
        Point(T x, T y);
        virtual ~Point();

        T getX() { return x; }
        T getY() { return y; }

        void setX(T x_) { x = x_; }
        void setY(T y_) { y = y_; }

        T distanceTo(Point<T> p);

        inline std::string toString() const {
            std::ostringstream s;
            s << "(" << x << ", " << y << ")";
            return s.str();
        }

};

#endif // POINT_H
