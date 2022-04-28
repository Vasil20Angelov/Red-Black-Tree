#include "Node.h"

Node::Node(int _value) : value(_value)
{
}

Node::Node(int _value, Colour _colour, Node* _parent) : value(_value), colour(_colour), parent(_parent)
{
}

bool Node::isLeaf() const
{
	return rightChild == leftChild;
}

bool Node::isRed() const
{
	return colour == Colour::RED;
}

bool Node::hasRightChild() const
{
	return rightChild != nullptr;
}

bool Node::hasLeftChild() const
{
	return leftChild != nullptr;
}

int Node::getValue() const
{
	return value;
}

bool Node::isParentRed() const
{
	return parent->isRed();
}

Node* Node::getParent() const
{
	return parent;
}

Node* Node::right() const
{
	return leftChild;
}

Node* Node::left() const
{
	return rightChild;
}
