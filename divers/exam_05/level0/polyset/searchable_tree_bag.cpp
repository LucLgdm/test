
#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() {}

searchable_tree_bag::~searchable_tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &other) {
	*this = other;
}

searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &other) {
	if (this != &other) {
		this->tree = other.tree;
	}
	return *this;
}

bool searchable_tree_bag::has(int value) const {
	std::function<bool(node*)> search = [&](node* n) {
        if (!n) return false;
        if (n->value == value) return true;
        return search(n->l) || search(n->r);
    };
    return search(tree);
}

