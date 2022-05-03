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

	bool deletingLeft;
	Node* parent = _node->parent;

	if (parent) {
		deletingLeft = _node->isLeftChild();
	}

	if (_node) {
		delete _node;
		_node = nullptr;
		--allocated;
		assert(allocated >= 0);
	}
	// Log deleting

	if (parent) {
		parent->nullChild(deletingLeft);
	}
}
