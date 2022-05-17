#pragma once
#include <cassert>
#include "Node.h"
#include "Logger.h"

class Allocator
{
public:
	//! Creates and returns a new node
	Node* allocate(int _value, Node* _parent = nullptr, Colour _colour = Colour::RED);
	//! Deletes the whole tree with root a given node
	void deAllocate(Node*& _node);
	//! Get the number of currently allocated nodes by this allocator 
	size_t getCurrentlyAllocated() const;

private:

	size_t allocated = 0;
};