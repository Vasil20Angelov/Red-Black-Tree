#include "RedBlackTree.h"

template<class Alloc>
RBT<Alloc>::RBT() : root(nullptr)
{
}

template<class Alloc>
RBT<Alloc>::RBT(int value)
{
	root = allocator.allocate(value, nullptr, Colour::BLACK);
}

template<class Alloc>
RBT<Alloc>::~RBT()
{
	allocator.deAllocate(root);
}

template<class Alloc>
void RBT<Alloc>::insert(int _value)
{
	Node* newNode = Operation::insert(root, allocator, _value);
	if (newNode && newNode != root)
		Operation::fixInsertion(newNode, root);
}

template<class Alloc>
void RBT<Alloc>::erase(int key)
{
	Operation::erase(root, allocator, key);
}

template<class Alloc>
bool RBT<Alloc>::contains(int key) const
{
	return Operation::contains(key);
}

template<class Alloc>
bool RBT<Alloc>::empty() const
{
	return allocator.getCurrentlyAllocated() == 0;
}

template<class Alloc>
size_t RBT<Alloc>::size() const
{
	return allocator.getCurrentlyAllocated();
}

template<class Alloc>
size_t RBT<Alloc>::getHeight() const
{
	return Operation::findHeight(root);
}

template<class Alloc>
void RBT<Alloc>::clear()
{
	allocator.deallocate(root);
}
