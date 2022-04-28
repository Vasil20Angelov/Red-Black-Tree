#include "Allocator.h"

Allocator::Allocator(int value) : node(new Node(value)), allocated(1)
{
}

Allocator::~Allocator()
{
	clear(node);
}

int Allocator::getCurrentlyAllocated() const
{
	return allocated;
}

void Allocator::clear(Node* _node)
{
	if (_node == nullptr)
		return;

	clear(_node->left());
	clear(_node->right());

	delete _node;
	_node = nullptr;
	--allocated;
}
