#include "Vect2.hpp"

// Constructeur
Vect2::Vect2(): _x(0), _y(0) {}

Vect2::~Vect2() {}

Vect2::Vect2(const Vect2 &other) {
    *this = other;
}

Vect2::Vect2(const int x, const int y): _x(x), _y(y){ }

Vect2 &Vect2::operator=(const Vect2 &other) {
    if (this != &other) {
        this->_x = other._x;
		this->_y = other._y;
    }
    return *this;
}

int &Vect2::operator[](const int index) {
	if (index == 0) return _x;
	return _y;
}

const int &Vect2::operator[](const int index) const{
	if (index == 0) return _x;
	return _y;
}


Vect2 &Vect2::operator++() {
	++_x;
	++_y;
	return *this;
}

Vect2 Vect2::operator++(int factice) {
	Vect2 tmp = *this;
	++_x;
	++_y;
	return tmp;
}

Vect2 &Vect2::operator--() {
	--_x;
	--_y;
	return *this;
}

Vect2 Vect2::operator--(int factice) {
	Vect2 tmp = *this;
	--_x;
	--_y;
	return tmp;
}

Vect2 &Vect2::operator+=(Vect2 other) {
	this->_x += other._x;
	this->_y += other._y;
	return *this;
}

Vect2 &Vect2::operator-=(Vect2 other) {
	this->_x -= other._x;
	this->_y -= other._y;
	return *this;
}

Vect2 &Vect2::operator*=(const int lambda) {
	this->_x *= lambda;
	this->_y *= lambda;
	return *this;
}

Vect2 Vect2::operator*(const int lambda) const {
	return Vect2(_x * lambda, _y * lambda);
}

Vect2 Vect2::operator-() const {
	return Vect2(-_x, -_y);
}

ostream &operator<<(ostream &os, const Vect2 &vec) {
	os << "{" << vec[0] << ", " << vec[1] << "}";
	return os;
}

Vect2 operator+(const Vect2 &a, const Vect2& b) {
	return Vect2(a.get_x() + b.get_y(), a.get_y() + b.get_y());
}

Vect2 operator-(const Vect2& a, const Vect2& b) {
	return Vect2(a.get_x() - b.get_x(), a.get_y() - b.get_y());
}


Vect2 operator*(int lambda, const Vect2& v) {
    return v * lambda;
}

bool operator==(const Vect2& a, const Vect2& b) {
	if (a.get_x() == b.get_x() && a.get_y() == b.get_y())
		return true;
	return false;
}

bool operator!=(const Vect2& a, const Vect2& b) {
	return !(a == b);
}