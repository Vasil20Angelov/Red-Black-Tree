#include "catch2/catch_all.hpp"

#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"
#include "Operations.h"
#include "RedBlackTree.cpp"

void createTree(Node*& root, Allocator& alc)
{
	Operation::insert(root, alc, 10);
	Operation::insert(root, alc, 20);
	Operation::insert(root, alc, 5);
	Node* lastAdded = Operation::insert(root, alc, 25);
	Operation::fixInsertion(lastAdded, root);
	lastAdded = Operation::insert(root, alc, 30);
	Operation::fixInsertion(lastAdded, root);
	Operation::insert(root, alc, 3);
	lastAdded = Operation::insert(root, alc, 1);
	Operation::fixInsertion(lastAdded, root);
	lastAdded = Operation::insert(root, alc, 35);
	Operation::fixInsertion(lastAdded, root);
	lastAdded = Operation::insert(root, alc, 32);
	Operation::fixInsertion(lastAdded, root);
	lastAdded = Operation::insert(root, alc, 27);
	Operation::fixInsertion(lastAdded, root);
	lastAdded = Operation::insert(root, alc, 29);
	Operation::fixInsertion(lastAdded, root);
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
		Node* e = alc.allocate(18, c);
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
	Node* r = alc.allocate(5);
	
	SECTION("Left")
	{
		Node* a = alc.allocate(10, r);
		Node* b = alc.allocate(20, a);
		Node* c = alc.allocate(30, b);
		Node* d = alc.allocate(15, b);

		Rotation::left(b);

		REQUIRE(b->left == a);
		REQUIRE(b->right == c);
		REQUIRE(a->right == d);
		REQUIRE(r->right == b);
		REQUIRE(b->parent == r);
		REQUIRE(a->parent == b);
		REQUIRE(c->parent == b);
		REQUIRE(d->parent == a);
		REQUIRE(c->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(b);
	}

	SECTION("Right")
	{
		Node* a = alc.allocate(4, r);
		Node* b = alc.allocate(2, a);
		Node* c = alc.allocate(1, b);
		Node* d = alc.allocate(3, b);

		Rotation::right(b);

		REQUIRE(b->left == c);
		REQUIRE(b->right == a);
		REQUIRE(a->left == d);
		REQUIRE(r->left == b);
		REQUIRE(b->parent == r);
		REQUIRE(a->parent == b);
		REQUIRE(c->parent == b);
		REQUIRE(d->parent == a);
		REQUIRE(c->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(b);
	}

	SECTION("Left-Right")
	{
		Node* a = alc.allocate(4, r);
		Node* b = alc.allocate(1, a);
		Node* c = alc.allocate(3, b);
		Node* d = alc.allocate(2, c);

		Rotation::leftRight(b);

		REQUIRE(c->left == b);
		REQUIRE(c->right == a);
		REQUIRE(b->right == d);
		REQUIRE(r->left == c);
		REQUIRE(c->parent == r);
		REQUIRE(a->parent == c);
		REQUIRE(b->parent == c);
		REQUIRE(d->parent == b);
		REQUIRE(a->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(c);
	}

	SECTION("Right-Left")
	{
		Node* a = alc.allocate(6, r);
		Node* b = alc.allocate(9, a);
		Node* c = alc.allocate(7, b);
		Node* d = alc.allocate(8, c);

		Rotation::rightLeft(b);

		REQUIRE(c->left == a);
		REQUIRE(c->right == b);
		REQUIRE(b->left == d);
		REQUIRE(r->right == c);
		REQUIRE(c->parent == r);
		REQUIRE(a->parent == c);
		REQUIRE(b->parent == c);
		REQUIRE(d->parent == b);
		REQUIRE(a->isLeaf() == true);
		REQUIRE(d->isLeaf() == true);

		alc.deAllocate(c);
	}

	alc.deAllocate(r);
}

TEST_CASE("Operations")
{
	Allocator alc;
	Node* root = nullptr;
	Node* lastAdded = nullptr;
	SECTION("Insert") {
		Operation::insert(root, alc, 10);
		SECTION("Inserting in empty tree")
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

		SECTION("Inserting new nodes are placed correctly")
		{
			REQUIRE(root->right->value == 20);
			REQUIRE(root->left->value == 5);
			REQUIRE(root->right->isRedColoured() == true);
			REQUIRE(root->left->isRedColoured() == true);
			REQUIRE(alc.getCurrentlyAllocated() == 3);
		}

		lastAdded = Operation::insert(root, alc, 25);
		Operation::fixInsertion(lastAdded, root);

		/* After fix:
		*              10(B)
		*              / \
		*          (B)5   20(B)
		*                   \
		*                   25(R)
		*/

		SECTION("Inserting correctly recolours nodes when no rotations are needed")
		{
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == false);
			REQUIRE(root->right->isRedColoured() == false);
			REQUIRE(lastAdded->isRedColoured() == true);
			REQUIRE(lastAdded->value == 25);
		}

		lastAdded = Operation::insert(root, alc, 30);
		Operation::fixInsertion(lastAdded, root);

		/* After fix:
		*              10(B)
		*              / \
		*          (B)5   25(B)
		*                 / \
		*            (R)20   30(R)
		*/

		SECTION("Inserting performs Left rotation correctly")
		{
			REQUIRE(lastAdded->isLeaf() == true);
			REQUIRE(lastAdded->isRightChild() == true);
			REQUIRE(lastAdded->isRedColoured() == true);
			REQUIRE(lastAdded->parent->isRedColoured() == false);
			REQUIRE(lastAdded->parent->left->isRedColoured() == true);
		}

		Operation::insert(root, alc, 3);
		lastAdded = Operation::insert(root, alc, 1);
		Operation::fixInsertion(lastAdded, root);

		/* After fix:
		*               10B
		*             /     \
		*           3B       25B
		*          /  \     /  \
		*        1R   5R  20R   30R
		*/

		SECTION("Inserting performs Right rotation correctly")
		{
			REQUIRE(lastAdded->isLeaf() == true);
			REQUIRE(lastAdded->isLeftChild() == true);
			REQUIRE(lastAdded->isRedColoured() == true);
			REQUIRE(lastAdded->parent->isRedColoured() == false);
			REQUIRE(lastAdded->parent->right->isRedColoured() == true);
		}

		lastAdded = Operation::insert(root, alc, 35);
		Operation::fixInsertion(lastAdded, root);

		/* After fix:
		*               10B
		*             /     \
		*           3B       25R
		*          /  \     /  \
		*        1R   5R  20B   30B
		*                         \
		*                         35R
		*/

		SECTION("Inserting recolours recursively without rotation")
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
		Operation::fixInsertion(lastAdded, root);

		/* After fix:
		*               10B
		*             /     \
		*           3B       25R
		*          /  \     /  \
		*        1R   5R  20B   32B
		*                      /  \
		*                    30R   35R
		*/

		SECTION("Inserting performs Right-left rotation correctly")
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
		Operation::fixInsertion(lastAdded, root);
		
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

		SECTION("Inserting performs Red-uncle case followed by right rotation in the middle of the tree")
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
		Operation::fixInsertion(lastAdded, root);		

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

		SECTION("Inserting performs Left-right rotation correctly")
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

	createTree(root, alc);
	SECTION("Searching")
	{
		REQUIRE(Operation::contains(root, 1) == true);
		REQUIRE(Operation::contains(root, 10) == true);
		REQUIRE(Operation::contains(root, 25) == true);
		REQUIRE(Operation::contains(root, 29) == true);
		REQUIRE(Operation::contains(root, 8) == false);
		REQUIRE(Operation::contains(root, 28) == false);
		REQUIRE(Operation::contains(root, 36) == false);
	}

	SECTION("Utilities")
	{
		SECTION("Find max")
		{
			REQUIRE(Operation::max(root) == 35);
			REQUIRE(Operation::max(root->left) == 20);
		}

		SECTION("Find min")
		{
			REQUIRE(Operation::min(root) == 1);
			REQUIRE(Operation::min(root->right) == 27);
		}

		SECTION("Find height")
		{
			REQUIRE(Operation::findHeight(root) == 4);
			REQUIRE(Operation::findHeight(root->left) == 3);
			REQUIRE(Operation::findHeight(root->left->right) == 1);
			REQUIRE(Operation::findHeight(root->left->right->right) == 0);
		}
	}

	SECTION("Erasing")
	{
		SECTION("Trying to erase non-existing node throws an exception")
		{
			size_t allocated = alc.getCurrentlyAllocated();
			REQUIRE_THROWS_AS(Operation::erase(root, alc, 50), std::logic_error);
			REQUIRE(allocated == alc.getCurrentlyAllocated());
		}

		Operation::erase(root, alc, 1);

		/* After erase:
		*               25B
		*             /     \
		*           10R      32R
		*          /  \      /  \
		*        3B   20B  29B  35B
		*         \        /  \
		*          5R    27R  30R
		*/

		SECTION("Removes a red leaf without changing anthing else")
		{
			Node* temp = root->left->left;
			REQUIRE(temp->value == 3);
			REQUIRE(temp->isRedColoured() == false);
			REQUIRE(temp->left == nullptr);
			REQUIRE(temp->right->isRedColoured() == true);
		}

		Operation::erase(root, alc, 25);
		Operation::erase(root, alc, 3);

		/* After erase:
		*               27B
		*             /     \
		*           10R      32R
		*          /  \      /  \
		*        5B   20B  29B  35B
		*                    \
		*                    30R
		*/

		SECTION("Swaps the value with the smallest element which is bigger than the node we want to delete and removes it as it is red")
		{
			REQUIRE(root->value == 27);
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == true);
			REQUIRE(root->right->isRedColoured() == true);
			REQUIRE(root->left->isRedColoured() == true);
			REQUIRE(root->right->right->isRedColoured() == false);
			REQUIRE(root->right->left->isRedColoured() == false);
			REQUIRE(root->left->right->isRedColoured() == false);
			REQUIRE(root->left->left->isRedColoured() == false);
			REQUIRE(root->left->left->value == 5);
			REQUIRE(root->left->left->isLeaf() == true);
			REQUIRE(root->right->left->value == 29);
			REQUIRE(root->right->left->hasLeftChild() == false);
			REQUIRE(root->right->left->hasRightChild() == true);
		}

		Operation::erase(root, alc, 27);

		/* After erase:
		*               29B
		*             /     \
		*           10R      32R
		*          /  \      /  \
		*        5B   20B  30B  35B                 
		*/

		SECTION("Double swap performed")
		{
			REQUIRE(root->value == 29);
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == true);
			REQUIRE(root->right->isRedColoured() == true);
			REQUIRE(root->right->left->isRedColoured() == false);
			REQUIRE(root->right->left->value == 30);
			REQUIRE(root->right->left->isLeaf() == true);
		}

		Operation::erase(root, alc, 5);

		/* After erase:
		*               29B
		*             /     \
		*           10B      32R
		*             \      /  \
		*             20R  30B  35B
		*/

		SECTION("Sibling has 2 black children (nulls)")
		{
			REQUIRE(root->value == 29);
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == false);
			REQUIRE(root->right->isRedColoured() == true);
			REQUIRE(root->left->right->isRedColoured() == true);
			REQUIRE(root->left->hasLeftChild() == false);
			REQUIRE(root->left->value == 10);
			REQUIRE(root->left->right->value == 20);
		}

		Operation::erase(root, alc, 20); // removes 20 without any recolourings and rotations
		Operation::erase(root, alc, 10);
		
		/* After erase:
		*               32B
		*              /   \
		*            29B    35B
		*              \
		*               30R
		*/

		SECTION("Red sibling case, followed by 2 black children case")
		{
			REQUIRE(root->value == 32);
			REQUIRE(root->left->value == 29);
			REQUIRE(root->right->value == 35);
			REQUIRE(root->left->right->value == 30);
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == false);
			REQUIRE(root->right->isRedColoured() == false);
			REQUIRE(root->left->right->isRedColoured() == true);
			REQUIRE(root->left->hasLeftChild() == false);
		}

		Operation::erase(root, alc, 35);		

		/* After erase:
		*               30B
		*              /   \
		*            29B    32B
		*/

		SECTION("'Near child is red' case, followed by 'Far child is red' case")
		{
			REQUIRE(root->value == 30);
			REQUIRE(root->left->value == 29);
			REQUIRE(root->right->value == 32);
			REQUIRE(root->isRedColoured() == false);
			REQUIRE(root->left->isRedColoured() == false);
			REQUIRE(root->right->isRedColoured() == false);
			REQUIRE(root->left->isLeaf() == true);
			REQUIRE(root->right->isLeaf() == true);
		}

		Operation::erase(root, alc, 30);
		Operation::erase(root, alc, 32);

		REQUIRE(root->value == 29);
		REQUIRE(root->isLeaf() == true);
		REQUIRE(root->isRedColoured() == false);

		Operation::erase(root, alc, 29);
		REQUIRE(root == nullptr);
	}

	alc.deAllocate(root);
	REQUIRE(alc.getCurrentlyAllocated() == 0);
}

TEST_CASE("RBT")
{
	RBT tree(4);
	tree.insert(3);
	tree.insert(2);

	REQUIRE(tree.getHeight() == 2);
}