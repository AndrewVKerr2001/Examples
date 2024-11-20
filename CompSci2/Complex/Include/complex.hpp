#include <iostream>

#ifndef __COMPLEX_HPP_
#define __COMPLEX_HPP_

class Complex {

private:
	double _real;
	double _imaginary;
	
public:
	Complex();
	Complex(double _r, double _i);
	~Complex();
	
public:
	void print() const;
	void print(std::ostream&) const;
	
public:
	Complex operator+(Complex&);
	Complex operator-(Complex&);
	Complex operator*(Complex&);
	Complex operator*(double);
	
public:
	friend std::ostream& operator<<(std::ostream&, const Complex&);
	
};
#endif
