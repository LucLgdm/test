#pragma once
#include <bits/stdc++.h>
using namespace std;


class Bigint {
	public:
		Bigint();
		~Bigint();
		Bigint(unsigned int);
		Bigint(const string&);
		Bigint(const Bigint &other);
		std::string getnum() const {return _str;}
		
		Bigint &operator=(const Bigint &other);
		Bigint &operator+=(Bigint);
		Bigint &operator++();
		Bigint operator++(int);
		Bigint operator<<(int n);
		Bigint operator>>(int n);
		Bigint &operator<<=(int n);
		Bigint &operator>>=(int n);

		Bigint operator>>(const Bigint& n);
		Bigint &operator>>=(const Bigint& n);
		Bigint operator<<(const Bigint& n) const;
		Bigint &operator<<=(const Bigint& n);
		


	private:
		std::string _str;
};

std::ostream &operator<<(std::ostream &os, const Bigint i);

Bigint operator+(const Bigint& a, const Bigint& b);
bool operator<(const Bigint& a, const Bigint& b);
bool operator>(const Bigint& a, const Bigint& b);
bool operator==(const Bigint& a, const Bigint& b);
bool operator!=(const Bigint& a, const Bigint& b);
bool operator<=(const Bigint& a, const Bigint& b);
bool operator>=(const Bigint& a, const Bigint& b);
