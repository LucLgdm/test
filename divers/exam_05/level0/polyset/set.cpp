
#include "set.hpp"

set::~set() { }

set::set(searchable_bag &bag): bag_ref(bag) { }

void set::insert(int value) {
	if (!bag_ref.has(value))
		bag_ref.insert(value);
}

bool set::has(int value) {
	return bag_ref.has(value);
}

void set::clear() {
	bag_ref.clear();
}

void set::print() {
	bag_ref.print();
}
searchable_bag &set::get_bag() {return bag_ref;}