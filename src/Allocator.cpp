#include "Allocator.h"

std::shared_ptr<Logger> Allocator::fileLogger = Logger::getInstance();

Node* Allocator::allocate(int _value, Node* _parent, Colour _colour)
{
	Node* newNode = nullptr;
	try {
		newNode = new Node(_value, _parent, _colour);
		++allocated;
		fileLogger->debug("Created a node with value: " + std::to_string(_value));
	}
	catch (std::exception& e) {
		fileLogger->error(e.what());
	}

	fileLogger->debug("Allocated nodes: " + std::to_string(allocated));
	return newNode;
}

void Allocator::deAllocate(Node*& _node)
{
	if (_node == nullptr)
		return;

	deAllocate(_node->left);
	deAllocate(_node->right);

	bool deletingLeft;
	Node* parent = _node->parent;

	if (parent) {
		deletingLeft = _node->isLeftChild();
	}

	int value = _node->value;
	delete _node;
	_node = nullptr;
	--allocated;
	assert(allocated >= 0);
	
	fileLogger->debug("Deleted node with value: " + std::to_string(value));
	fileLogger->debug("Allocated nodes: " + std::to_string(allocated));

	if (parent) {
		parent->nullChild(deletingLeft);
	}
}

size_t Allocator::getCurrentlyAllocated() const
{
	return allocated;
}
