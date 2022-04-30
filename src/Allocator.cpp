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

void Allocator::deAllocate(Node*& _node)
{
	clear(_node);
}

size_t Allocator::getCurrentlyAllocated() const
{
	return allocated;
}

void Allocator::clear(Node*& _node)
{
	if (_node == nullptr)
		return;

	clear(_node->left);
	clear(_node->right);

	Node* temp = _node->parent;
	int value = _node->value;

	delete _node;
	_node = nullptr;

	if (temp) {
		if (value < temp->value)
			temp->left = nullptr;
		else
			temp->right = nullptr;
	}

	--allocated;
}
