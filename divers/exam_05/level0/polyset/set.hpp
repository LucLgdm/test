
#pragma once
#include "searchable_bag.hpp"

class set {
	public:
		set() = delete;
		~set();
		set(const set &) = delete;
		set(searchable_bag &);

		set &operator=(const set &) = delete;
		searchable_bag &get_bag();

		void insert(int n);
		bool has(int n);
		void clear();
		void print();
	
	private:
		searchable_bag &bag_ref;
};