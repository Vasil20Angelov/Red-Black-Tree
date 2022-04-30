#include "catch2/catch_all.hpp"
#include "Allocator.h"
#include "RedBlackTree.h"
#include "Rotation.h"
#include "Operations.h"
#include "Node.h"

Node* findRoot(Node* _node)
{
	if (_node->parent)
		return findRoot(_node->parent);

	return _node;
}

TEST_CASE("Allocator")
{
	Allocator alc;
	Node* a = alc.allocate(10);
	Node* b = alc.allocate(15, a, Colour::BLACK);
	Node* c = alc.allocate(20, b);
	Node* d = alc.allocate(8, a, Colour::RED);

	SECTION("Corectly creates new nodes")
	{
		REQUIRE(a->value == 10);
		REQUIRE(b->value == 15);
		REQUIRE(c->value == 20);
		REQUIRE(d->value == 8);

		REQUIRE(a->parent == nullptr);
		REQUIRE(b->parent == a);
		REQUIRE(c->parent == b);
		REQUIRE(d->parent == a);

		REQUIRE(a->left == d);
		REQUIRE(a->right == b);
		REQUIRE(b->right == c);
		REQUIRE(b->left == nullptr);
		REQUIRE(c->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		REQUIRE(a->colour == Colour::RED);
		REQUIRE(b->colour == Colour::BLACK);
		REQUIRE(c->colour == Colour::RED);
		REQUIRE(d->colour == Colour::RED);

		REQUIRE(alc.getCurrentlyAllocated() == 4);
	}

	SECTION("Correctly deletes a node")
	{
		Node* e = alc.allocate(50, c);
		alc.deAllocate(e);

		REQUIRE(e == nullptr);
		REQUIRE(alc.getCurrentlyAllocated() == 4);
		REQUIRE(c->isLeaf() == true);
	}

	SECTION("Correctly deletes a node and all it's heirs")
	{
		alc.deAllocate(a);
		REQUIRE(a == nullptr);
		REQUIRE(alc.getCurrentlyAllocated() == 0);
	}
}

TEST_CASE("Rotations")
{
	Allocator alc;
	Node* root = alc.allocate(5);
	
	SECTION("Left")
	{
		Node* a = alc.allocate(10, root);
		Node* b = alc.allocate(20, a);
		Node* c = alc.allocate(30, b);
		Node* d = alc.allocate(15, b);

		Rotation::left(c);

		REQUIRE(b->left == a);
		REQUIRE(b->right == c);
		REQUIRE(a->right == d);
		REQUIRE(root->right == b);
		REQUIRE(b->parent == root);
		REQUIRE(a->parent == b);
		REQUIRE(c->parent == b);
		REQUIRE(d->parent == a);
		REQUIRE(c->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(b);
	}

	SECTION("Right")
	{
		Node* a = alc.allocate(4, root);
		Node* b = alc.allocate(2, a);
		Node* c = alc.allocate(1, b);
		Node* d = alc.allocate(3, b);

		Rotation::right(c);

		REQUIRE(b->left == c);
		REQUIRE(b->right == a);
		REQUIRE(a->left == d);
		REQUIRE(root->left == b);
		REQUIRE(b->parent == root);
		REQUIRE(a->parent == b);
		REQUIRE(c->parent == b);
		REQUIRE(d->parent == a);
		REQUIRE(c->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(b);
	}

	SECTION("Left-Right")
	{
		Node* a = alc.allocate(4, root);
		Node* b = alc.allocate(1, a);
		Node* c = alc.allocate(3, b);
		Node* d = alc.allocate(2, c);

		Rotation::leftRight(c);

		REQUIRE(c->left == b);
		REQUIRE(c->right == a);
		REQUIRE(b->right == d);
		REQUIRE(root->left == c);
		REQUIRE(c->parent == root);
		REQUIRE(a->parent == c);
		REQUIRE(b->parent == c);
		REQUIRE(d->parent == b);
		REQUIRE(a->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(c);
	}

	SECTION("Right-Left")
	{
		Node* a = alc.allocate(6, root);
		Node* b = alc.allocate(9, a);
		Node* c = alc.allocate(7, b);
		Node* d = alc.allocate(8, c);

		Rotation::rightLeft(c);

		REQUIRE(c->left == a);
		REQUIRE(c->right == b);
		REQUIRE(b->left == d);
		REQUIRE(root->right == c);
		REQUIRE(c->parent == root);
		REQUIRE(a->parent == c);
		REQUIRE(b->parent == c);
		REQUIRE(d->parent == b);
		REQUIRE(a->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(c);
	}
}

TEST_CASE("Operations")
{
	Allocator alc;
	Node* root = nullptr;
	Node* lastAdded = nullptr;
	SECTION("Insertion")
	{
		Operation::insert(root, alc, 10);
		SECTION("Empty tree")
		{
			REQUIRE(root->value == 10);
			REQUIRE(root->isLeaf() == true);
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(alc.getCurrentlyAllocated() == 1);
		}

		Operation::insert(root, alc, 20);
		Operation::insert(root, alc, 5);

		/*
		*              10(B)
		*              / \
		*          (R)5   20(R)
		*/

		SECTION("New nodes are placed correctly")
		{
			REQUIRE(root->right->value == 20);
			REQUIRE(root->left->value == 5);
			REQUIRE(root->right->isRedColoured() == true);
			REQUIRE(root->left->isRedColoured() == true);
			REQUIRE(alc.getCurrentlyAllocated() == 3);
		}

		lastAdded = Operation::insert(root, alc, 25);
		Operation::fix(lastAdded, root);

		/* After fix:
		*              10(B)
		*              / \
		*          (B)5   20(B)
		*                   \
		*                   25(R)
		*/

		SECTION("Correctly recolouring the nodes")
		{
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == false);
			REQUIRE(root->right->isRedColoured() == false);
			REQUIRE(lastAdded->isRedColoured() == true);
			REQUIRE(lastAdded->value == 25);
		}

		SECTION("Proper rotation and recolouring is performed")
		{
			lastAdded = Operation::insert(root, alc, 30);
			Operation::fix(lastAdded, root);
	
			/* After fix:
			*              10(B)
			*              / \
			*          (B)5   25(B)
			*                 / \
			*            (R)20   30(R)
			*/

			SECTION("Left rotation")
			{
				REQUIRE(lastAdded->isLeaf() == true);
				REQUIRE(lastAdded->isRightChild() == true);
				REQUIRE(lastAdded->isRedColoured() == true);
				REQUIRE(lastAdded->parent->isRedColoured() == false);
				REQUIRE(lastAdded->parent->left->isRedColoured() == true);
			}
			
			Operation::insert(root, alc, 3);
			lastAdded = Operation::insert(root, alc, 1);
			Operation::fix(lastAdded, root);

			/* After fix:
			*               10B
			*             /     \
			*           3B       25B
			*          /  \     /  \
			*        1R   5R  20R   30R
			*/

			SECTION("Right rotation")
			{
				REQUIRE(lastAdded->isLeaf() == true);
				REQUIRE(lastAdded->isLeftChild() == true);
				REQUIRE(lastAdded->isRedColoured() == true);
				REQUIRE(lastAdded->parent->isRedColoured() == false);
				REQUIRE(lastAdded->parent->right->isRedColoured() == true);
			}

			lastAdded = Operation::insert(root, alc, 35);
			Operation::fix(lastAdded, root);

			/* After fix:
			*               10B
			*             /     \
			*           3B       25R
			*          /  \     /  \
			*        1R   5R  20B   30B
			*                         \
			*                         35R
			*/

			SECTION("Recolour without rotation")
			{
				REQUIRE(lastAdded->isLeaf() == true);
				REQUIRE(lastAdded->isRightChild() == true);
				REQUIRE(lastAdded->isRedColoured() == true);
				REQUIRE(lastAdded->parent->isRedColoured() == false);
				REQUIRE(lastAdded->parent->parent->isRedColoured() == true);
				REQUIRE(lastAdded->isUncleRed() == false);
				REQUIRE(root->isRedColoured() == false);
			}

			lastAdded = Operation::insert(root, alc, 32);
			Operation::fix(lastAdded, root);

			/* After fix:
			*               10B
			*             /     \
			*           3B       25R
			*          /  \     /  \
			*        1R   5R  20B   32B
			*                      /  \
			*                    30R   35R
			*/

			SECTION("Right-left rotation")
			{
				REQUIRE(lastAdded->left->value == 30);
				REQUIRE(lastAdded->right->value == 35);
				REQUIRE(lastAdded->left->isRedColoured() == true);
				REQUIRE(lastAdded->right->isRedColoured() == true);
				REQUIRE(lastAdded->parent->isRedColoured() == true);
				REQUIRE(lastAdded->isRedColoured() == false);
				REQUIRE(lastAdded->isRightChild() == true);
			}

			lastAdded = Operation::insert(root, alc, 27);
			Operation::fix(lastAdded, root);
			root = findRoot(root);

			/* After fix:
			*               25B
			*             /     \
			*           10R      32R
			*          /  \      /  \
			*        3B   20B  30B  35B
			*       / \        /                                  
			*      1R  5R     27R      
			*                 
			*/

			SECTION("Red-uncle case performed followed by right rotation in the middle of the tree")
			{
				REQUIRE(root->value == 25);
				REQUIRE(root->left->value == 10);
				REQUIRE(root->right->value == 32);
				REQUIRE(root->isRedColoured() == false);
				REQUIRE(root->right->isRedColoured() == true);
				REQUIRE(root->left->isRedColoured() == true);
				REQUIRE(root->left->right->isRedColoured() == false);
				REQUIRE(lastAdded->parent->value == 30);
				REQUIRE(lastAdded->parent->isRedColoured() == false);
				REQUIRE(lastAdded->isUncleRed() == false);
			}

			lastAdded = Operation::insert(root, alc, 29);
			Operation::fix(lastAdded, root);
			root = findRoot(root);

			/* After fix:
			*               25B
			*             /     \
			*           10R      32R
			*          /  \      /  \
			*        3B   20B  29B  35B
			*       / \        /  \
			*      1R  5R    27R  30R
			*
			*/

			SECTION("Left-right rotation")
			{
				REQUIRE(lastAdded->isRedColoured() == false);
				REQUIRE(lastAdded->parent->value == 32);
				REQUIRE(lastAdded->parent->isRedColoured() == true);
				REQUIRE(lastAdded->left->isRedColoured() == true);
				REQUIRE(lastAdded->right->isRedColoured() == true);
			}

			SECTION("Adding a node with the same value as other node in the tree throws an exception")
			{
				size_t allocated = alc.getCurrentlyAllocated();
				REQUIRE_THROWS_AS(Operation::insert(root, alc, 10), std::logic_error);
				REQUIRE_THROWS_AS(Operation::insert(root, alc, 27), std::logic_error);
				REQUIRE(allocated == alc.getCurrentlyAllocated());
			}

			alc.deAllocate(root);
			REQUIRE(alc.getCurrentlyAllocated() == 0);
		}
	}

	SECTION("Searching")
	{
		Operation::insert(root, alc, 10);
		Operation::insert(root, alc, 25);
		Operation::insert(root, alc, 14);
		Operation::insert(root, alc, 3);
		Operation::insert(root, alc, 8);
		Operation::insert(root, alc, 53);
		Operation::insert(root, alc, 22);

		REQUIRE(Operation::contains(root, 10) == true);
		REQUIRE(Operation::contains(root, 22) == true);
		REQUIRE(Operation::contains(root, 3) == true);
		REQUIRE(Operation::contains(root, 1) == false);
		REQUIRE(Operation::contains(root, 15) == false);

		alc.deAllocate(root);
		REQUIRE(alc.getCurrentlyAllocated() == 0);
	}

}