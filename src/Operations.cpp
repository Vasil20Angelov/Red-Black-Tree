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

void Operation::fixInsertion(Node* current, const Node* root)
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
	return find(root, key) != nullptr;
}

void Operation::erase(Node*& root, Allocator& allocator, int key)
{
	Node* toDel = find(root, key);
	if (!toDel) {
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

void Operation::redSibling(Node*& sibling)
{
	std::swap(sibling->colour, sibling->parent->colour);

	if (sibling->isRightChild())
		Rotation::left(sibling);
	else
		Rotation::right(sibling);
}

void Operation::blackChildren(Node*& sibling, Node*& DB)
{
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
	if (sibling->isRightChild()) {
		std::swap(sibling->colour, sibling->left->colour);
		Rotation::right(sibling->left);
	}
	else {
		std::swap(sibling->colour, sibling->right->colour);
		Rotation::left(sibling->right);
	}
}

void Operation::farChildIsRed(Node*& sibling)
{
	std::swap(sibling->colour, sibling->parent->colour);

	if (sibling->isRightChild()) {
		sibling->right->colour = Colour::BLACK;
		Rotation::left(sibling);
	}
	else {
		sibling->left->colour = Colour::BLACK;
		Rotation::right(sibling);
	}
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
}

Node* Operation::replace(Node*& node)
{
	if (node->hasLeftChild() && !node->hasRightChild()) {
		node->value = node->left->value;
		node->left->value = node->value - 1;
		if (!node->left->isLeaf())
			return replace(node->left);

		return node->left;
	}

	Node* minInLeft = min(node->right);
	node->value = minInLeft->value;
	minInLeft->value = node->value + 1;
	if (minInLeft->hasRightChild())
		return replace(minInLeft);

	return minInLeft;
}

Node* Operation::max(Node* root)
{
	if (root->right)
		return max(root->right);

	return root;
}

Node* Operation::min(Node* root)
{
	if (root->left)
		return min(root->left);

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

	//Node*& temp = root;
	//while (temp) {
	//	if (temp->value > key)
	//		temp = temp->left;
	//	else if (temp->value < key)
	//		temp = temp->right;
	//	else
	//	 break;
	//}
	//return temp;
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
		Rotation::right(parent);
		return parent;
	}

	if (_node->isRightChild() && parent->isRightChild()) {
		Rotation::left(parent);
		return parent;
	}

	if (_node->isLeftChild() && parent->isRightChild()) {
		Rotation::rightLeft(parent);
		return _node;
	}

	if (_node->isRightChild() && parent->isLeftChild())
		Rotation::leftRight(parent);

	return _node;
}
