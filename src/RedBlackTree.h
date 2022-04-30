#pragma once
#include <stdexcept>
#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"
#include "Operations.h"

#define alloc allocator.alloc;
#define deAlloc allocator.deAllocate

template <class Alloc = Allocator> 
class RBT {

public:
	RBT();
	RBT(int value);
	~RBT();

	void insert(int _value);
	bool contains(int key) const;

private:

	Node* root;
	Alloc allocator;
};
