#include "Allocator.h"

Node* Allocator::allocate(int _value, Node* _parent, Colour _colour)
{
	Node* newNode = nullptr;
	try {
		newNode = new Node(_value, _parent, _colour);
		++allocated;
	}
	catch (...) {
		// Log it
	}

	return newNode;
}

void Allocator::deAllocate(Node* _node)
{
	clear(_node);
}

int Allocator::getCurrentlyAllocated() const
{
	return allocated;
}

void Allocator::clear(Node* _node)
{
	if (_node == nullptr)
		return;

	clear(_node->left);
	clear(_node->right);

	if (_node->parent) {
		if (_node->value < _node->parent->value)
			_node->parent->left = nullptr;
		else
			_node->parent->right = nullptr;
	}

	delete _node;
	_node = nullptr;
	--allocated;
}
