#pragma once
#include "Node.h"

class Rotation {

public:

	/**
	* \brief Makes a left rotation
	* \param The node from where the rotation begins
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
	static void left(Node* c)
	{
		Node* a = c->parent->parent;
		changeParentConnection(a, c);

		// If b has a left child it should be moved as a right child of a
		a->right = c->parent->left;
		if (a->right)
			a->right->parent = a;

		c->parent->left = a;
	}

	/**
	* \brief Makes a right rotation
	* \param The node from where the rotation begins
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
	static void right(Node* c)
	{
		Node* a = c->parent->parent;
		changeParentConnection(a, c);

		// If b has a right child it should be moved as a left child of a
		a->left = c->parent->right;
		if (a->left)
			a->left->parent = a;

		c->parent->right = a;
	}


	/**
	* \brief Makes a left-right rotation
	* \param The node from where the rotation begins
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
	static void leftRight(Node* c)
	{
		Node* b = c->parent;
		b->parent->left = c;
		c->parent = b->parent;

		// If c has a left child it should be moved as a right child of b
		b->right = c->left;
		if (b->right)
			b->right->parent = b;

		c->left = b;
		b->parent = c;

		right(b);
	}

	/**
	* \brief Makes a right-left rotation
	* \param The node from where the rotation begins
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
	static void rightLeft(Node* c)
	{
		Node* b = c->parent;
		b->parent->right = c;
		c->parent = b->parent;

		// If c has a right child it should be moved as a left child of b
		b->left = c->right;
		if (b->left) 
			b->left->parent = b;

		c->right = b;
		b->parent = c;

		left(b);
	}

private:
	
	/**
	* @brief Changes the parents connections between a,c and the parent of a
	* 
	* The child of the parent of a should be changed from a to c and the parent of c should become the parent of a,
	* while the new parent of a should be c
	*/
	static void changeParentConnection(Node* a, Node* c)
	{
		Node* aPar = a->parent;
		if (aPar) {
			if (aPar->left == a)
				aPar->left = c->parent;
			else
				aPar->right = c->parent;
		}

		c->parent->parent = aPar;
		a->parent = c->parent;
	}
};