#pragma once
#include <cassert>
#include "Node.h"

class Allocator
{
public:
	Node* allocate(int _value, Node* _parent = nullptr, Colour _colour = Colour::RED);
	void deAllocate(Node* _node);
	int getCurrentlyAllocated() const;

private:
	void clear(Node* _node);

	int allocated = 0;
};