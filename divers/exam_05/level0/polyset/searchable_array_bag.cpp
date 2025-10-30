
#include "searchable_array_bag.hpp"

searchable_array_bag::searchable_array_bag() { }

searchable_array_bag::~searchable_array_bag() { }

searchable_array_bag::searchable_array_bag(const searchable_array_bag &other) {
	*this = other;
}

searchable_array_bag &searchable_array_bag::operator=(const searchable_array_bag &other) {
	if (this != &other) {
		this->data = other.data;
		this->size = other.size;
	}
	return *this;
}

bool searchable_array_bag::has(int value) const {
	for(int i = 0; i < this->size; i++) {
		if (data[i] == value) return true;
	}
	return false;
}


