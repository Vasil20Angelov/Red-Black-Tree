#pragma once
#include "Node.h"

class Allocator
{
public:
	Allocator() = default;
	Allocator(int value);
	// copy constr? & =
	~Allocator();

	Node* add_node(int value);
	int find_node(int value);
	int getCurrentlyAllocated() const;

private:
	void clear(Node* _node);

	Node* node = nullptr;
	int allocated = 0;
};