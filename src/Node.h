#pragma once

enum struct Colour : bool
{
	RED,
	BLACK
};

struct Node {

	Node() = default;
	Node(int _value) : value(_value) {};
	Node(int _value, Node* _parent, Colour _colour = Colour::RED) : value(_value), parent(_parent), colour(_colour) 
	{
		if (_parent) {
			if (_value < _parent->value)
				_parent->left = this;
			else
				_parent->right = this;
		}
	};

	bool operator== (const Node* other) { return this->value == other->value; }

	inline bool hasRightChild() const { return right != nullptr; }
	inline bool hasLeftChild()  const { return left  != nullptr; }
	inline bool isLeaf()	    const { return right == nullptr && left == nullptr; }


	// Data members
	int value = 0;
	Colour colour = Colour::BLACK;
	Node* parent = nullptr;
	Node* right = nullptr;
	Node* left = nullptr;
};