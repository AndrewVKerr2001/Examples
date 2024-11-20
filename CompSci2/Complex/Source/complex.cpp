#include "complex.hpp"

Complex::Complex() {
	this->_real = 0;
	this->_imaginary = 0;
}

Complex::Complex(double _r, double _i) {
	this->_real = _r;
	this->_imaginary = _i;
}

Complex::~Complex() {
	this->_real = 0;
	this->_imaginary = 0;
}

void Complex::print() const {
	this->print(std::cout);
}

void Complex::print(std::ostream& out) const {
	out << this->_real << (_imaginary < 0 ? "" : "+") << this->_imaginary << "i";
}

Complex Complex::operator+(Complex& other) {
	Complex answer;
	answer._real = _real + other._real;
	answer._imaginary = _imaginary + other._imaginary;
	return answer;
}

Complex Complex::operator-(Complex& other) {
	Complex answer;
	answer._real = _real - other._real;
	answer._imaginary = _imaginary - other._imaginary;
	return answer;
}

Complex Complex::operator*(Complex& other) {
	Complex answer;
	answer._real = _real * other._real - _imaginary * other._imaginary;
	answer._imaginary = _imaginary * other._real + _real * other._imaginary;
	return answer;
}

Complex Complex::operator*(double other) {
	Complex answer;
	answer._real = _real * other;
	answer._imaginary = _imaginary * other;
	return answer;
}


std::ostream& operator<<(std::ostream& out, const Complex& complex) {
	complex.print(out);
	return out;
}

