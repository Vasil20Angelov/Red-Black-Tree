#pragma once
#include <cassert>
#include "Node.h"

class Allocator
{
public:
	//! Creates and returns a new node
	Node* allocate(int _value, Node* _parent = nullptr, Colour _colour = Colour::RED);
	//! Deletes a given node
	void deAllocate(Node*& _node);
	//! Get the number of currently allocated nodes by this allocator 
	size_t getCurrentlyAllocated() const;

private:
	//! Deletes the whole tree with root a given node
	void clear(Node*& _node);

	size_t allocated = 0;
};