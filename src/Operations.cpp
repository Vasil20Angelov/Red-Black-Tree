#include "Operations.h"

Node* Operation::insert(Node*& root, Allocator& allocator, int _value)
{
	if (!root) {
		root = allocator.allocate(_value, nullptr, Colour::BLACK);
		return root;
	}

	Node* parent = insertRec(_value, root);

	// If temp is nullptr then a node with that value is found (not allowing multiple nodes with the same value)
	if (!parent) {
		throw std::logic_error("Cannot add multiple nodes with the same value!\n");
		return nullptr;
	}

	Node* newNode = allocator.allocate(_value, parent, Colour::RED);

	return newNode;
}

void Operation::fix(Node* current, const Node* root)
{
	Node* parent = current->parent;
	while (parent->isRedColoured() && current->isRedColoured())
	{
		Node* uncleNode = current->getUncle();
		if (!uncleNode->isRedColoured()) {
			Node* subRoot = rotate(current);

			if (subRoot == parent)  // LL or RR rotation has been performed
				recolour(parent, current);
			else  // LR or RL rotation has been performed so current is the subRoot
				recolour(current, parent);
			
			break;
		}

		uncleNode->colour = Colour::BLACK;
		parent->colour = Colour::BLACK;
		if (parent->parent == root)
			break;

		parent->parent->colour = Colour::RED;
		current = parent->parent;
		parent = current->parent;
	}
}

bool Operation::contains(Node* root, int key)
{
	if (!root)
		return false;

	if (root->value > key)
		return contains(root->left, key);

	if (root->value < key)
		return contains(root->right, key);

	return true;
}

void recolour(Node*& node1, Node*& node2)
{
	node1->colour = Colour::BLACK;
	if (node2->isLeftChild())
		node1->right->colour = Colour::RED;
	else
		node1->left->colour = Colour::RED;
}

Node* insertRec(int _value, Node* _node)
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

Node* rotate(Node* _node)
{
	Node* parent = _node->parent;
	if (_node->isLeftChild() && parent->isLeftChild()) {
		Rotation::right(_node);
		return parent;
	}

	if (_node->isRightChild() && parent->isRightChild()) {
		Rotation::left(_node);
		return parent;
	}

	if (_node->isLeftChild() && parent->isRightChild()) {
		Rotation::rightLeft(_node);
		return _node;
	}

	if (_node->isRightChild() && parent->isLeftChild())
		Rotation::leftRight(_node);

	return _node;
}
