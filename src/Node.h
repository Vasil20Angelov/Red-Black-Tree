#pragma once
#include <vector>

enum struct Colour : bool
{
	RED,
	BLACK
};

class Node {

public:
	Node() = default;
	Node(int _value) : value(_value) {};
	Node(int _value, Node*& _parent, Colour _colour = Colour::RED) : value(_value), parent(_parent), colour(_colour) 
	{
		if (_parent) {
			if (_value < _parent->value)
				_parent->left = this;
			else
				_parent->right = this;
		}
	};

	bool operator==(const Node*& other) const { return areSame(this, other); }
	bool operator!=(const Node*& other) const { return !(this == other); }

	std::vector<int> inOrderPrint() const
	{
		std::vector<int> vec;
		inOrderPrintRec(this, vec);
		return vec;
	}

	void nullChild(bool leftChild)
	{
		if (leftChild)
			left = nullptr;
		else
			right = nullptr;
	}

	Node* getUncle() const 
	{ 
		Node* grandP = parent->parent;
		if (parent->isLeftChild())
			return grandP->right;

		return grandP->left;
	}

	inline Node* getSibling()   const { return isLeftChild() ? parent->right : parent->left; }
	inline bool hasRightChild() const { return right != nullptr; }
	inline bool hasLeftChild()  const { return left  != nullptr; }
	inline bool isLeaf()	    const { return right == nullptr && left == nullptr; }
	inline bool isLeftChild()   const { return this == parent->left; }
	inline bool isRightChild()	const { return this == parent->right; }
	inline bool isRedColoured()	const { return this != nullptr && colour == Colour::RED; }
	inline bool isUncleRed()	const { return getUncle()->isRedColoured(); }
	inline bool compare(const Node* other) const { return areSame(this, other); }

private:
	void inOrderPrintRec(const Node* node, std::vector<int>& vec) const
	{
		if (!node)
			return;

		inOrderPrintRec(node->left, vec);
		vec.push_back(node->value);
		inOrderPrintRec(node->right, vec);
	}

	bool areSame(const Node* node1, const Node* node2) const
	{
		if (!node1 && !node2)
			return true;

		if (node1 && node2 && node1->value == node2->value && node1->colour == node2->colour)
			return areSame(node1->left, node2->left) && areSame(node1->right, node2->right);

		return false;
	}

public:
	// Data members
	int value = 0;
	Colour colour = Colour::BLACK;
	Node* parent = nullptr;
	Node* right = nullptr;
	Node* left = nullptr;
};