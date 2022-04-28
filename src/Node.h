#pragma once

enum struct Colour : bool
{
	RED,
	BLACK
};

class Node {
public:
	Node() = default;
	Node(int _value);
	Node(int _value, Colour _colour, Node* _parent);
	
	int  getValue() const;
	bool isLeaf() const;
	bool isRed() const;
	bool hasRightChild() const;
	bool hasLeftChild() const;

	bool isParentRed() const;
	Node* getParent() const;
	Node* right() const;
	Node* left() const;

private:
	int value = 0;
	Colour colour = Colour::BLACK;
	Node* parent = nullptr;
	Node* rightChild = nullptr;
	Node* leftChild = nullptr;
};