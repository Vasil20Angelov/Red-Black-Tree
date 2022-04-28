#include "RedBlackTree.h"

RBT::RBT() : root(nullptr)
{
}

RBT::RBT(int value)
{
	root = new Node(value);
}

RBT::~RBT()
{
	delete root;
}

bool RBT::leaf() const
{
	return root->isLeaf();
}

int RBT::value() const
{
	return root->getValue();
}

