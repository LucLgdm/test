#include "Bigint.hpp"

// Constructeur
Bigint::Bigint(): _x("0") {}

Bigint::~Bigint() {}

Bigint::Bigint(const Bigint &other) {
    *this = other;
}

Bigint::Bigint(unsigned int num) {
	this->_x = std::to_string(num);
}

Bigint &Bigint::operator=(const Bigint &other) {
    if (this != &other) {
        this->_x = other._x;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Bigint i) {
	os << i.getnum();
	return os;
}
