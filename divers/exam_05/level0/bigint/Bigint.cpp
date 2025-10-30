#include "Bigint.hpp"

// Constructeur
Bigint::Bigint(): _str("0") {}

Bigint::~Bigint() {}

Bigint::Bigint(const Bigint &other) {
	*this = other;
}

Bigint::Bigint(unsigned int num) {
	this->_str = std::to_string(num);
}

Bigint::Bigint(const string& s) {
	size_t pos = s.find_first_not_of('0');
	_str = (pos == std::string::npos) ? "0" : s.substr(pos);
}

Bigint &Bigint::operator=(const Bigint &other) {
	if (this != &other) {
		this->_str = other._str;
	}
	return *this;
}

Bigint &Bigint::operator+=(Bigint other) {
	*this = *this + other;
	return *this;
}

Bigint& Bigint::operator++() {
	*this += Bigint(1);
	return *this;
}


Bigint Bigint::operator++(int factice) {
	Bigint tmp(*this);
	++(*this);
	return tmp;
}

Bigint Bigint::operator<<(int n) {
	Bigint res = *this;
	res._str.append(n, '0');
	return res;
}

Bigint Bigint::operator>>(int n) {
	Bigint res = *this;

	if (n < 0) return res;
	if (n >= (int)_str.size()) {
		res._str = "0";
		return res;
	}
	res._str.resize(_str.size() - n);
	return res;
}

Bigint& Bigint::operator<<=(int n) {
	*this = *this << n;
	return *this;
}

Bigint& Bigint::operator>>=(int n) {
	*this = *this >> n;
	return *this;
}


Bigint Bigint::operator>>(const Bigint& n) {
	int shift = std::min((int)std::stoul(n.getnum()), (int)_str.size());
	Bigint res = *this;

	if (shift <= 0)
		return res;

	if ((size_t)shift >= _str.size())
		res._str = "0";
	else
		res._str.resize(_str.size() - shift);
	return res;
}

Bigint& Bigint::operator>>=(const Bigint& n) {
	*this = *this >> n;
	return *this;
}

Bigint Bigint::operator<<(const Bigint& n) const {
	int shift = std::stoi(n.getnum());
	return *this << shift;
}

Bigint& Bigint::operator<<=(const Bigint& n) {
	*this = *this << n;
	return *this;
}



std::ostream &operator<<(std::ostream &os, const Bigint i) {
	os << i.getnum();
	return os;
}

Bigint operator+(const Bigint& a, const Bigint& b) {
	string A = a.getnum();
	string B = b.getnum();

	if (A.size() < B.size()) swap(A, B);
	B.insert(B.begin(), A.size() - B.size(), '0');
	
	int retenu = 0;
	string res;

	for(int i = A.size() - 1; i >= 0; --i) {
		int sum = A[i] - '0' + B[i] - '0' + retenu;
		retenu = sum / 10;
		int digit = sum % 10;
		res.insert(res.begin(), '0' + digit);
	}

	if (retenu) res.insert(res.begin(), '0' + retenu);

	return Bigint(res);

	/* Plus rapide, on remplit a l'envers et on inverse
	std::string res_rev;
	for (int i = A.size() - 1; i >= 0; --i) {
		int sum = (A[i]-'0') + (B[i]-'0') + retenu;
		retenu = sum / 10;
		res_rev.push_back('0' + sum % 10);
	}
	if (retenu) res_rev.push_back('0' + retenu);
	std::reverse(res_rev.begin(), res_rev.end());
	return Bigint(res_rev);
	*/
}

bool operator<(const Bigint& a, const Bigint& b) {
	string A = a.getnum();
	string B = b.getnum();

	A.erase(0, A.find_first_not_of('0'));
	B.erase(0, B.find_first_not_of('0'));

	if (A.size() != B.size())
		return A.size() < B.size();
	return A < B;
}

bool operator>(const Bigint& a, const Bigint& b) {
	return b < a;
}

bool operator==(const Bigint& a, const Bigint& b) {
	string A = a.getnum();
	string B = b.getnum();

	A.erase(0, A.find_first_not_of('0'));
	B.erase(0, B.find_first_not_of('0'));
	return A == B;
}

bool operator!=(const Bigint& a, const Bigint& b) {
	return !(a == b);
}

bool operator<=(const Bigint& a, const Bigint& b) {
	return !(b < a);
}

bool operator>=(const Bigint& a, const Bigint& b) {
	return !(a < b);
}


