#pragma once
#include <stdexcept>
#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"

class Operation
{
public:
	static Node* insert(Node*& root, Allocator& allocator, int _value);
	static void fixInsertion(Node* current, const Node* root);
	static bool contains(Node* root, int key);
	static void erase(Node*& root, Allocator& allocator, int key);
	
	static void redSibling(Node*& sibling);
	static void blackChildren(Node*& sibling, Node*& DB);
	static void nearChildIsRed(Node*& sibling);
	static void farChildIsRed(Node*& sibling);

private:
	
	static void fixDeletion(Node*& root, Node*& node);
	static Node* replace(Node*& _node);
	static Node* max(Node* root);
	static Node* min(Node* root);
	static Node* find(Node*& root, int key);
	static void  recolour(Node*& node1, Node*& node2);
	static Node* insertRec(int _value, Node* _node);
	static Node* rotate(Node* _node);
};