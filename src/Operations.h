#pragma once
#include <stdexcept>
#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"
#include "Logger.h"
class Operation
{

public:

	//! Creates and inserts a node to a given tree
	static Node* BSTinsert(Node*& root, Allocator& allocator, int _value, Colour colour = Colour::RED);
	//! After inserting, rotates or recoulours the tree to be in correct state
	static void balancedInsert(Node*& root, Allocator& allocator, int _value);
	//! Erases a node from the tree
	static void erase(Node*& root, Allocator& allocator, int key);
	//! Checks if the tree contains a certain node
	static bool contains(Node* root, int key);

	//! Finds the height of a given tree
	static size_t findHeight(Node* root);
	//! Finds the biggest element in a given tree
	static int max(Node* root);
	//! Finds the smallest element in a given tree
	static int min(Node* root);

private:
	
	static Node* findMax(Node* root);
	static Node* findMin(Node* root);
	static Node* find(Node*& root, int key);
	static Node* rotate(Node* _node);
	static Node* replace(Node*& _node);
	static Node* insertRec(int _value, Node* _node);
	static void  recolour(Node*& node1, Node*& node2);
	static void  fixDeletion(Node*& root, Node*& node);

	//! Fixes the tree in case: the node that is being erased has a red sibling
	static void redSibling(Node*& sibling);
	//! Fixes the tree in case: the node that is being erased has a black sibling with both children not red (black or null)
	static void blackChildren(Node*& sibling, Node*& DB);
	//! Fixes the tree in case: the node that is being erased has a black sibling and the near child to the node is red
	static void nearChildIsRed(Node*& sibling);
	//! Fixes the tree in case: the node that is being erased has a black sibling and the far child from the node is red
	static void farChildIsRed(Node*& sibling);
};