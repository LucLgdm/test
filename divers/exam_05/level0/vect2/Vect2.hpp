
#pragma once

#include <iostream>
using namespace std;

class Vect2 {
	public:
		Vect2();
		Vect2(const int x, const int y);
		~Vect2();
		Vect2(const Vect2 &);
		int get_x() const {return _x;}
		int get_y() const {return _y;}
		void set_x(int a) {this->_x = a;}
		void set_y(int a) {this->_y = a;}
		
		Vect2 &operator=(const Vect2 &);
		int &operator[](const int index);
		const int &operator[](const int index) const;

		Vect2 &operator++();
		Vect2 operator++(int);
		Vect2 &operator--();
		Vect2 operator--(int);
		Vect2 &operator+=(Vect2);
		Vect2 &operator-=(Vect2);
		Vect2 &operator*=(const int lambda);

		Vect2 operator*(const int lambda) const;
		Vect2 operator-() const;
		

	private:
		int _x;
		int _y;
};

ostream& operator<<(ostream &os, const Vect2 &vec);
Vect2 operator+(const Vect2& a, const Vect2& b);
Vect2 operator-(const Vect2& a, const Vect2& b);
Vect2 operator*(int lambda, const Vect2& v);
bool operator==(const Vect2& a, const Vect2& b);
bool operator!=(const Vect2& a, const Vect2& b);