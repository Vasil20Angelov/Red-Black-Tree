#include "catch2/catch_all.hpp"
#include "Allocator.h"
#include "RedBlackTree.h"
#include "Rotation.h"
#include "Node.h"

TEST_CASE("Allocator")
{
	Allocator alc;
	Node* a = alc.allocate(10);
	Node* b = alc.allocate(15, a);
	Node* c = alc.allocate(20, b);
	Node* d = alc.allocate(8, a);

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

		REQUIRE(alc.getCurrentlyAllocated() == 4);
	}

	SECTION("Correctly deletes a node")
	{
		Node* e = alc.allocate(50, c);
		alc.deAllocate(e);

		REQUIRE(alc.getCurrentlyAllocated() == 4);
		REQUIRE(c->isLeaf() == true);
	}

	SECTION("Correctly deletes a node and all it's heirs")
	{
		alc.deAllocate(a);
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
