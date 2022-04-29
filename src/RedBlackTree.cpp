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
Node* RBT<Alloc>::insert(int _value)
{
	if (!root) {
		root = alloc(_value, nullptr, Colour::BLACK);
		return root;
	}

	Node* parent = insertRec(_value, node);

	// If temp is nullptr then a node with that value is found (not allowing multiple nodes with the same value)
	if (!parent)
		return nullptr;

	Node* newNode = alloc(_value, parent, Colour::RED);

	return newNode;
}

template<class Alloc>
Node* RBT<Alloc>::find(int _value) const
{
	return findRec(_value, node);
}

template<class Alloc>
Node* RBT<Alloc>::insertRec(int _value, Node* _node)
{
	if (_node->value == _value)
		return nullptr;

	if (_node->value < _value) {
		if (_node->hasRightChild())
			return insertRec(_value, _node->right);
	}
	else { // _node->getValue() > _value
		if (_node->hasLeftChild())
			return insertRec(_value, _node->left);
	}

	return _node;
}

template<class Alloc>
Node* RBT<Alloc>::findRec(int _value, Node* _node) const
{
	if (!_node)
		return nullptr;

	if (_node->value == _value)
		return _node;

	if (_node->value < _value)
		return findRec(_value, _node->right);

	return findRec(_value, _node->left);
}