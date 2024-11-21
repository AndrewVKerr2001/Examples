//Author: Andrew Kerr
//Date: 04/05/2022

#include <iostream>
#include <string>
#include <cmath>

#ifndef __AK_POINT_HPP__
#define __AK_POINT_HPP__

class Point {
private:
    //The label for the point.
    std::string _label;
    //The x position for the point.
    double _x;
    //The y position for the point.
    double _y;

public:
    //Constructs a blank (0,0) point.
    Point();
    //Constructs a point using the given information (used for debugging).
    Point(double x, double y, const std::string& label);

public:
    //Obtains a reference to the x coordinate value for modification.
    double& x();
    //Obtains a reference to the y coordinate value for modification.
    double& y();

public:
    //The x position of the point.
    double x() const;
    //The y position of the point.
    double y() const;
    //Obtains a constant reference to the label value.
    const std::string& label() const;

public:
    //Finds the distance between this point and the given point.
    double distance(const Point&) const;

public:
    //Reads in a point's values given an input stream.
    friend std::istream& operator>>(std::istream&, Point&);
    //Outputs the point's values to the given output stream.
    friend std::ostream& operator<<(std::ostream&, const Point&);

public:
    //A simple INFINITE point that points to (INFINITY, INFINITY).
    static const Point& INFINITE();
};

#endif//__AK_POINT_HPP__