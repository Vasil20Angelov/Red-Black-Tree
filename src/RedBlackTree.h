#pragma once
#include "Node.h"
#include "Allocator.h"

#define alloc allocator.alloc;
#define deAlloc allocator.deAllocate

template <class Alloc = Allocator> 
class RBT {

public:
	RBT();
	RBT(int value);
	~RBT();

	Node* insert(int _value);
	Node* find(int _value) const;

private:
	Node* insertRec(int _value, Node* node);
	Node* findRec(int _value, Node* _node) const;

	Node* root;
	Alloc allocator;
};
