//Author: Andrew Kerr
//Date: 04/05/2022

#include "Point.hpp"
#include <math.h>

Point::Point() {
    //Setup our x and y coordinates with zeros, label will be auto constructed to empty.
    _x = 0;
    _y = 0;
}

Point::Point(double x, double y, const std::string& label) : _label(label) {
    //Setup our x and y coordinates with the given values.
    _x = x;
    _y = y;
}

double& Point::x() {
    return _x;
}

double& Point::y() {
    return _y;
}

double Point::x() const {
    return _x;
}

double Point::y() const {
    return _y;
}

const std::string& Point::label() const{
    return _label;
}

double Point::distance(const Point& o) const {
    if(this == &o) return INFINITY;
    //Use the distance formula to find the distance between the two points.
    return sqrt(pow(_x - o._x, 2) + pow(_y - o._y, 2));
}

std::istream& operator>>(std::istream& i, Point& p) {
    //Read in our info from the given istream.
    i >> p._label;
    i >> p._x;
    i >> p._y;
    return i;
}

std::ostream& operator<<(std::ostream& o, const Point& p) {
    //Output our info to the given ostream.
    o << p._label << " (" << p._x << ", " << p._y << ")";
    return o;
}

const Point& Point::INFINITE(){
    //Construct a static function variable to be infinite and return it.
    static Point p(INFINITY, INFINITY, "INFINITE");
    return p;
}