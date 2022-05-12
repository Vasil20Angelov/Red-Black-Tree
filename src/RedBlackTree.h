#pragma once
#include <vector>

#include "Node.h"
#include "Allocator.h"
#include "Operations.h"

template <class Alloc = Allocator> 
class RBT {

public:
	RBT() : root(nullptr) { }
	RBT(int value) { root = allocator.allocate(value, nullptr, Colour::BLACK); }

	~RBT()
	{
		allocator.deAllocate(root);
		assert(empty() == true); 
	}

	inline void insert(int _value) { Operation::balancedInsert(root, allocator, _value); }

	inline void erase(int key) { Operation::erase(root, allocator, key); }

	inline void clear() { allocator.deAllocate(root); }

	inline bool contains(int key) const { return Operation::contains(root, key); }

	inline bool empty() const { return allocator.getCurrentlyAllocated() == 0; }

	inline size_t size() const { return allocator.getCurrentlyAllocated(); }

	inline size_t getHeight() const { return Operation::findHeight(root); }

	inline std::vector<int> printInOrder() { return root->inOrderPrint(); }

private:
	Node* root;
	Alloc allocator;
};
