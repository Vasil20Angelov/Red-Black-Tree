#pragma once
#include <stdexcept>
#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"

//#define alloc allocator.alloc;
//#define deAlloc allocator.deAllocate

class Operation
{
public:
	static Node* insert(Node*& root, Allocator& allocator, int _value);
	static void fix(Node* current, const Node* root);
	static bool contains(Node* root, int key);

private:
	friend void  recolour(Node*& node1, Node*& node2);
	friend Node* insertRec(int _value, Node* _node);
	friend Node* rotate(Node* _node);
};