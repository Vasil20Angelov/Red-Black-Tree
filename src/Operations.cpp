#include "Operations.h"

Node* Operation::BSTinsert(Node*& root, Allocator& allocator, int _value, Colour colour)
{
	if (!root) {
		root = allocator.allocate(_value, nullptr, Colour::BLACK);
		return root;
	}
	
	Node* parent = insertRec(_value, root);
	
	// If temp is nullptr then a node with that value is found (not allowing multiple nodes with the same value)
	if (!parent) {
		fileLogger->warn("Trying to add a node with the same value as another node ({0})", _value);
		throw std::logic_error("Cannot add multiple nodes with the same value!\n");
		return nullptr;
	}

	Node* newNode = allocator.allocate(_value, parent, colour);

	return newNode;
}

void Operation::balancedInsert(Node*& root, Allocator& allocator, int _value)
{
	Node* current = BSTinsert(root, allocator, _value);
	if (root == current)
		return;

	Node* parent = current->parent;
	while (parent->isRedColoured() && current->isRedColoured())
	{
		Node* uncleNode = current->getUncle();
		if (!uncleNode->isRedColoured()) {
			Node* subRoot = rotate(current);
			if (root->parent)
				root = root->parent;

			if (subRoot == parent)  // LL or RR rotation has been performed
				recolour(parent, current);
			else  // LR or RL rotation has been performed so current is the subRoot
				recolour(current, parent);
			
			break;
		}

		uncleNode->colour = Colour::BLACK;
		parent->colour = Colour::BLACK;
		if (parent->parent->compare(root))
			break;

		parent->parent->colour = Colour::RED;
		current = parent->parent;
		parent = current->parent;
	}
}

bool Operation::contains(Node* root, int key)
{
	return find(root, key) != nullptr;
}

void Operation::erase(Node*& root, Allocator& allocator, int key)
{
	Node* toDel = find(root, key);
	if (!toDel) {
		fileLogger->warn("Trying to delete non-existing node {0}", key);
		throw std::logic_error("There is not a node with that value in the tree!\n");
		return;
	}

	if (!toDel->isLeaf())
		toDel = replace(toDel);

	if (toDel == root) {
		allocator.deAllocate(root);
		return;
	}

	if (toDel->isRedColoured()) {
		allocator.deAllocate(toDel);
		return;
	}

	fixDeletion(root, toDel);
	allocator.deAllocate(toDel);
}

size_t Operation::findHeight(Node* root)
{
	if (root)
		return 1 + std::max(findHeight(root->right), findHeight(root->left));

	return 0;
}

int Operation::max(Node* root)
{
	return findMax(root)->value;
}

int Operation::min(Node* root)
{
	return findMin(root)->value;
}

void Operation::fixDeletion(Node*& root, Node*& toDel)
{
	Node* DB = toDel; // DB = Double black

	while (DB && DB != root)
	{
		Node* sibling = DB->getSibling();

		if (sibling->isRedColoured()) {
			redSibling(sibling);
			continue;
		}

		if (!sibling->right->isRedColoured() && !sibling->left->isRedColoured()) {
			blackChildren(sibling, DB);
			continue;
		}

		if (sibling->isRightChild() && sibling->left->isRedColoured() || sibling->isLeftChild() && sibling->right->isRedColoured()) {
			nearChildIsRed(sibling);
			sibling = sibling->parent;
		}

		farChildIsRed(sibling);
		break;
	}

	if (root->parent)
		root = root->parent;
}

void Operation::redSibling(Node*& sibling)
{
	fileLogger->trace("Entered case: Sibling is red coloured");

	std::swap(sibling->colour, sibling->parent->colour);

	if (sibling->isRightChild()) {
		fileLogger->trace("Executing LEFT rotation on node with value {0}", sibling->value);
		Rotation::left(sibling);
	}
	else {
		fileLogger->trace("Executing LEFT rotation on node with value {0}", sibling->value);
		Rotation::right(sibling);
	}
}

void Operation::blackChildren(Node*& sibling, Node*& DB)
{
	fileLogger->trace("Entered case: Sibling is black coloured and both it's children are black or null");

	sibling->colour = Colour::RED;
	if (sibling->parent->isRedColoured()) {
		sibling->parent->colour = Colour::BLACK;
		DB = nullptr;
	}
	else
		DB = sibling->parent;
}

void Operation::nearChildIsRed(Node*& sibling)
{
	fileLogger->trace("Entered case: Sibling is black coloured and the near child is red");

	if (sibling->isRightChild()) {
		std::swap(sibling->colour, sibling->left->colour);
		fileLogger->trace("Executing RIGHT rotation on node with value {0}", sibling->left->value);
		Rotation::right(sibling->left);
	}
	else {
		std::swap(sibling->colour, sibling->right->colour);
		fileLogger->trace("Executing left rotation on node with value {0}", sibling->right->value);
		Rotation::left(sibling->right);
	}
}

void Operation::farChildIsRed(Node*& sibling)
{
	fileLogger->trace("Entered case: Sibling is black coloured and far child is red");

	std::swap(sibling->colour, sibling->parent->colour);

	if (sibling->isRightChild()) {
		sibling->right->colour = Colour::BLACK;
		fileLogger->trace("Executing LEFT rotation on node with value {0}", sibling->value);
		Rotation::left(sibling);
	}
	else {
		sibling->left->colour = Colour::BLACK;
		fileLogger->trace("Executing RIGHT rotation on node with value {0}", sibling->value);
		Rotation::right(sibling);
	}
}

Node* Operation::replace(Node*& node)
{
	if (node->hasLeftChild() && !node->hasRightChild()) {
		node->value = node->left->value;
		if (!node->left->isLeaf())
			return replace(node->left);

		return node->left;
	}

	Node* minInLeft = findMin(node->right);
	node->value = minInLeft->value;
	if (minInLeft->hasRightChild())
		return replace(minInLeft);

	return minInLeft;
}

Node* Operation::findMax(Node* root)
{
	if (root->right)
		return findMax(root->right);

	return root;
}

Node* Operation::findMin(Node* root)
{
	if (root->left)
		return findMin(root->left);

	return root;
}

Node* Operation::find(Node*& root, int key)
{
	if (!root)
		return nullptr;

	if (root->value > key)
		return find(root->left, key);

	if (root->value < key)
		return find(root->right, key);

	return root;
}

void Operation::recolour(Node*& node1, Node*& node2)
{
	node1->colour = Colour::BLACK;
	if (node2->isLeftChild())
		node1->right->colour = Colour::RED;
	else
		node1->left->colour = Colour::RED;
}

Node* Operation::insertRec(int _value, Node* _node)
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

Node* Operation::rotate(Node* _node)
{
	Node* parent = _node->parent;
	if (_node->isLeftChild() && parent->isLeftChild()) {
		fileLogger->trace("Executing RIGHT rotation on node with value {0}", parent->value);
		Rotation::right(parent);
		return parent;
	}

	if (_node->isRightChild() && parent->isRightChild()) {
		fileLogger->trace("Executing LEFT rotation on node with value {0}", parent->value);
		Rotation::left(parent);
		return parent;
	}

	if (_node->isLeftChild() && parent->isRightChild()) {
		fileLogger->trace("Executing RIGHT-LEFT rotation on node with value {0}", parent->value);
		Rotation::rightLeft(parent);
		return _node;
	}

	fileLogger->trace("Executing LEFT-RIGHT rotation on node with value {0}", parent->value);
	Rotation::leftRight(parent);

	return _node;
}
