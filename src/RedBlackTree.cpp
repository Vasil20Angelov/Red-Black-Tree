#include "RedBlackTree.h"

template<class Alloc>
RBT<Alloc>::RBT() : root(nullptr)
{
}

template<class Alloc>
RBT<Alloc>::RBT(int value)
{
	root = alloc(value, nullptr, Colour::BLACK);
}

template<class Alloc>
RBT<Alloc>::~RBT()
{
	deAlloc(root);
}

template<class Alloc>
void RBT<Alloc>::insert(int _value)
{
	Node* newNode = Operation::insert(root, allocator, _value);
	if (newNode && newNode != root)
		Operation::fix(newNode, root);
}

template<class Alloc>
bool RBT<Alloc>::contains(int key) const
{
	return Operation::contains(key);
}
