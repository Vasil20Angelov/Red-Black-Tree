#pragma once
#include "Node.h"

struct Rotation {

	/**
	* \brief Makes a left rotation
	* \param The node from where the rotation begins
	* 
	* 	a						b
	*	 \					   / \
	*	  b			=>		  a   c
	*	   \
	*	    c
	* 
	*/
	static void left(Node* node)
	{
		Node* grandP = node->parent->parent;
		Node* grandPPar = grandP->parent;
		if (grandPPar) {
			if (grandPPar->left == grandP)
				grandPPar->left = node->parent;
			else
				grandPPar->right = node->parent;
		}

		node->parent->left = grandP;
		node->parent->parent = grandPPar;
		grandP->parent = node->parent;
		grandP->right = nullptr;
	}
};