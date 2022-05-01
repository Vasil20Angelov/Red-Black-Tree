#pragma once
#include "Node.h"

class Rotation {

public:

	/**
	* \brief Makes a left rotation
	* \param The middle node of the connection we want to rotate
	* 
	* The rotation is performed when there are 2 right-right connections
	* 
	* 	a                       b
	*	 \(right)              / \
	*	  b           =>      a   c
	*	   \(right)
	*	    c
	* 
	*/
	static void left(Node* b)
	{
		Node* a = b->parent;
		changeParentConnection(a, b);

		// If b has a left child it should be moved as a right child of a
		a->right = b->left;
		if (a->right)
			a->right->parent = a;

		b->left = a;
	}

	/**
	* \brief Makes a right rotation
	* \param The middle node of the connection we want to rotate
	*
	* The rotation is performed when there are 2 left-left connections
	* 
	*       a                       b
	*      /(left)                 / \
	*     b            =>         c   a
	*    /(left)
	*   c
	*
	*/
	static void right(Node* b)
	{
		Node* a = b->parent;
		changeParentConnection(a, b);

		// If b has a right child it should be moved as a left child of a
		a->left = b->right;
		if (a->left)
			a->left->parent = a;

		b->right = a;
	}


	/**
	* \brief Makes a left-right rotation
	* \param The middle node of the connection we want to rotate
	*
	* The rotation is performed when there are left-right connections
	*
	*       a                           c
	*      /(left)                     / \
	*     b                =>         b   a
	*      \(right)
	*       c
	*
	*/
	static void leftRight(Node* b)
	{
		Node* c = b->right;
		b->parent->left = c;
		c->parent = b->parent;

		// If c has a left child it should be moved as a right child of b
		b->right = c->left;
		if (b->right)
			b->right->parent = b;

		c->left = b;
		b->parent = c;

		right(c);
	}

	/**
	* \brief Makes a right-left rotation
	* \param The middle node of the connection we want to rotate
	*
	* The rotation is performed when there are right-left connections
	*
	*       a                           c
	*        \(right)                  / \
	*         b             =>        a   b
	*        /(left)		
	*       c
	*
	*/
	static void rightLeft(Node* b)
	{
		Node* c = b->left;
		b->parent->right = c;
		c->parent = b->parent;

		// If c has a right child it should be moved as a left child of b
		b->left = c->right;
		if (b->left) 
			b->left->parent = b;

		c->right = b;
		b->parent = c;

		left(c);
	}

private:
	
	/**
	* @brief Changes the parents connections between a,c and the parent of a
	* 
	* The child of the parent of a should be changed from a to c and the parent of c should become the parent of a,
	* while the new parent of a should be c
	*/
	static void changeParentConnection(Node* a, Node* b)
	{
		Node* aPar = a->parent;
		if (aPar) {
			if (aPar->left == a)
				aPar->left = b;
			else
				aPar->right = b;
		}

		b->parent = aPar;
		a->parent = b;
	}
};