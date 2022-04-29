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
	Node* root = alc.allocate(1);
	
	SECTION("Left rotation")
	{
		Node* a = alc.allocate(10, root);
		Node* b = alc.allocate(20, a);
		Node* c = alc.allocate(30, b);

		Rotation::left(c);

		REQUIRE(b->left == a);
		REQUIRE(b->right == c);
		REQUIRE(root->right == b);
		REQUIRE(b->parent == root);
		REQUIRE(a->parent == b);
		REQUIRE(c->parent == b);
		REQUIRE(a->isLeaf() == true);
		REQUIRE(c->isLeaf() == true);

		alc.deAllocate(b);
	}
}
