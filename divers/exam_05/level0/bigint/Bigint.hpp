#pragma once
#include <iostream>

class Bigint {
	public:
		Bigint();
		~Bigint();
		Bigint(unsigned int);
		Bigint(const Bigint &other);
		std::string getnum() const {return _x;}
		
		Bigint &operator=(const Bigint &other);
		Bigint &operator+=(Bigint);
		Bigint &operator++();
		Bigint operator++(int);
		Bigint &operator<<(int n);

	private:
		std::string _x;
};

std::ostream &operator<<(std::ostream &os, const Bigint i);

Bigint operator+(const Bigint& a, const Bigint& b);