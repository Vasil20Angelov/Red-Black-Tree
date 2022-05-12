#include <vector>
#include "catch2/catch_all.hpp"

#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"
#include "Operations.h"
#include "RedBlackTree.h"

using std::vector;

void createSimpleTree(Node*& root, Allocator& alc)
{
	root = alc.allocate(10);
	Node* a = alc.allocate(5, root, Colour::BLACK);
	Node* b = alc.allocate(15, root, Colour::BLACK);
	Node* c = alc.allocate(20, b, Colour::BLACK);
	Node* d = alc.allocate(1, a, Colour::RED);
	Node* e = alc.allocate(0, d, Colour::RED);
	Node* f = alc.allocate(2, d, Colour::BLACK);
}

 void createBalancedTree(Node*& root, Allocator& alc)
{
	Operation::balancedInsert(root, alc, 10);
	Operation::balancedInsert(root, alc, 20);
	Operation::balancedInsert(root, alc, 5);
	Operation::balancedInsert(root, alc, 25);
	Operation::balancedInsert(root, alc, 30);
	Operation::balancedInsert(root, alc, 3);
	Operation::balancedInsert(root, alc, 1);
	Operation::balancedInsert(root, alc, 35);
	Operation::balancedInsert(root, alc, 32);
	Operation::balancedInsert(root, alc, 27);
	Operation::balancedInsert(root, alc, 29);
}

Node*& createBSTTree(Allocator& alc, const vector<int>& values, const vector<Colour>& colours)
{
	Node* root = nullptr;
	for (int i = 0; i < values.size(); ++i) {
		Operation::BSTinsert(root, alc, values[i], colours[i]);
	}

	return root;
}

void CreateAndCompare(Node*& root, const vector<int>& values, const vector<Colour>& colours)
{
	Allocator alc;
	Node* expected = createBSTTree(alc, values, colours);
	REQUIRE(root->compare(expected));
	alc.deAllocate(expected);
}

void emptyTree(const RBT<>& tree)
{
	REQUIRE(tree.empty());
	REQUIRE(tree.size() == 0);
	REQUIRE(tree.getHeight() == 0);
}

void createRBTfromVector(RBT<>& tree, const vector<int>& vec)
{
	for (int v : vec)
		tree.insert(v);
}

void deleteInOrderFromRBT(RBT<>& tree, const vector<int>& vec)
{
	for (int v : vec)
		tree.erase(v);
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

TEST_CASE("Node")
{
	Allocator alc1;
	Node* root1;

	createSimpleTree(root1, alc1);

	SECTION("In order traversing")
	{
		vector<int> expected{ 0, 1, 2, 5, 10, 15, 20 };
		REQUIRE(expected == root1->inOrderPrint());
	}

	SECTION("Compare")
	{
		Allocator alc2;
		Node* root2;

		SECTION("Same trees")
		{
			createSimpleTree(root2, alc2);
			REQUIRE(root1->compare(root2) == true);
			REQUIRE(root2->compare(root1) == true);
		}

		SECTION("Different structures")
		{		
			root2 = alc2.allocate(10);
			Node* a = alc2.allocate(5, root2, Colour::BLACK);
			Node* b = alc2.allocate(15, root2, Colour::BLACK);
			Node* c = alc2.allocate(20, b, Colour::BLACK);
			Node* d = alc2.allocate(1, c, Colour::RED);
			Node* e = alc2.allocate(0, c, Colour::RED);
			REQUIRE(root2->compare(root1) == false);
			REQUIRE(root1->compare(root2) == false);
			alc2.deAllocate(root2);
		}

		SECTION("Different value and any node")
		{
			root2 = alc2.allocate(10);
			Node* a = alc2.allocate(5, root2, Colour::BLACK);
			Node* b = alc2.allocate(15, root2, Colour::BLACK);
			Node* c = alc2.allocate(20, b, Colour::BLACK);
			Node* d = alc2.allocate(1, a, Colour::RED);
			Node* e = alc2.allocate(-1, d, Colour::RED);
			Node* f = alc2.allocate(2, d, Colour::BLACK);
			REQUIRE(root2->compare(root1) == false);
			REQUIRE(root1->compare(root2) == false);
			alc2.deAllocate(root2);
		}

		SECTION("Different colour and any node")
		{
			root2 = alc2.allocate(10);
			Node* a = alc2.allocate(5, root2, Colour::BLACK);
			Node* b = alc2.allocate(15, root2, Colour::BLACK);
			Node* c = alc2.allocate(20, b, Colour::BLACK);
			Node* d = alc2.allocate(1, a, Colour::RED);
			Node* e = alc2.allocate(-1, d, Colour::RED);
			Node* f = alc2.allocate(2, d, Colour::RED);
			REQUIRE(root2->compare(root1) == false);
			REQUIRE(root1->compare(root2) == false);
			alc2.deAllocate(root2);
		}

	}

	alc1.deAllocate(root1);
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
	Allocator alc, alc2;
	Node* root = nullptr;
	Node* expected = nullptr;

	SECTION("BSTinsert")
	{
		Operation::BSTinsert(root, alc, 10);
		Operation::BSTinsert(root, alc, 5);
		Operation::BSTinsert(root, alc, 6);
		Operation::BSTinsert(root, alc, 2);
		Operation::BSTinsert(root, alc, 23);
		Operation::BSTinsert(root, alc, 3);
		Operation::BSTinsert(root, alc, 1);
		Operation::BSTinsert(root, alc, 20);
		Operation::BSTinsert(root, alc, 54);

		SECTION("Inserting in the correct place")
		{
			vector<int> expected{ 1,2,3,5,6,10,20,23,54 };
			REQUIRE(root->inOrderPrint() == expected);
		}

		SECTION("Trying to insert an existing element throws an exception")
		{
			REQUIRE_THROWS_AS(Operation::BSTinsert(root, alc, 10), std::logic_error);
			REQUIRE_THROWS_AS(Operation::BSTinsert(root, alc, 2), std::logic_error);
		}

		REQUIRE(alc.getCurrentlyAllocated() == 9);
		alc.deAllocate(root);
	}

	SECTION("balancedInsert") 
	{
		Operation::balancedInsert(root, alc, 10);
		SECTION("Inserting in empty tree")
		{
			REQUIRE(root->value == 10);
			REQUIRE(root->isLeaf() == true);
			REQUIRE(root->isRedColoured() == false);
		}

		Operation::balancedInsert(root, alc, 20);
		Operation::balancedInsert(root, alc, 5);

		/*
		*              10(B)
		*              / \
		*          (R)5   20(R)
		*/

		vector<int> values{ 10,5,20 };
		vector<Colour> colours{ Colour::BLACK, Colour::RED, Colour::RED };

		SECTION("New nodes are given red colour")
		{
			expected = createBSTTree(alc2, values, colours);

			REQUIRE(expected->compare(root));
			
			alc2.deAllocate(expected);
		}

		Operation::balancedInsert(root, alc, 25);

		/* After fix:
		*              10(B)
		*              / \
		*          (B)5   20(B)
		*                   \
		*                   25(R)
		*/

		values.push_back(25);
		colours[1] = Colour::BLACK;
		colours[2] = Colour::BLACK;
		colours.push_back(Colour::RED);
		// values: 10, 5, 20, 25
		// colours: B, B, B, R

		SECTION("Inserting correctly recolours nodes when no rotations are needed")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::balancedInsert(root, alc, 30);

		/* After fix:
		*              10(B)
		*              / \
		*          (B)5   25(B)
		*                 / \
		*            (R)20   30(R)
		*/

		std::swap(values[2], values[3]);
		values.push_back(30);
		colours.push_back(Colour::RED);
		// values: 10, 5, 25, 20, 30
		// colours: B, B, B, R, R

		SECTION("Inserting performs Left rotation correctly")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::balancedInsert(root, alc, 3);
		Operation::balancedInsert(root, alc, 1);

		/* After fix:
		*               10B
		*             /     \
		*           3B       25B
		*          /  \     /  \
		*        1R   5R  20R   30R
		*/

		values[1] = 3;
		values.push_back(1);
		values.push_back(5);
		colours.push_back(Colour::RED);
		colours.push_back(Colour::RED);
		// values: 10, 3, 20, 25, 30, 1, 5
		// colours: B, B, B, R, R, R

		SECTION("Inserting performs Right rotation correctly")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::balancedInsert(root, alc, 35);

		/* After fix:
		*               10B
		*             /     \
		*           3B       25R
		*          /  \     /  \
		*        1R   5R  20B   30B
		*                         \
		*                         35R
		*/

		values.push_back(35);
		colours.push_back(Colour::RED);
		colours[2] = Colour::RED;
		colours[3] = Colour::BLACK;
		colours[4] = Colour::BLACK;
		// values: 10, 3, 20, 25, 30, 1, 5, 35
		// colours: B, B, B, R, B, R, R, R

		SECTION("Inserting recolours recursively without rotation")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::balancedInsert(root, alc, 32);

		/* After fix:
		*               10B
		*             /     \
		*           3B       25R
		*          /  \     /  \
		*        1R   5R  20B   32B
		*                      /  \
		*                    30R   35R
		*/

		values[4] = 32;
		values.push_back(30);
		colours.push_back(Colour::RED);
		// values: 10, 3, 20, 25, 32, 1, 5, 35, 30
		// colours: B, B, B, R, B, R, R, R, R

		SECTION("Inserting performs Right-left rotation correctly")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::balancedInsert(root, alc, 27);
		
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

		values[0] = 25;
		colours[0] = Colour::BLACK;
		values[1] = 10;
		colours[1] = Colour::RED;
		values[2] = 3;
		colours[2] = Colour::BLACK;
		values[3] = 1;
		colours[3] = Colour::RED;
		values[4] = 5;
		colours[4] = Colour::RED;
		values[5] = 20;
		colours[5] = Colour::BLACK;
		values[6] = 32;
		colours[6] = Colour::RED;
		values[7] = 30;
		colours[7] = Colour::BLACK;
		values[8] = 27;
		colours[8] = Colour::RED;
		values.push_back(35);
		colours.push_back(Colour::BLACK);
		// values: 25, 10, 3, 1, 5, 20, 32, 30, 27, 35
		// colours: B, R, B, R, R, B, R, B, R, B


		SECTION("Inserting performs Red-uncle case followed by right rotation in the middle of the tree")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::balancedInsert(root, alc, 29);		

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

		values[7] = 29;
		values.push_back(30);
		colours.push_back(Colour::RED);
		// values: 25, 10, 3, 1, 5, 20, 32, 29, 27, 35, 30
		// colours: B, R, B, R, R, B, R, B, R, B, R

		SECTION("Inserting performs Left-right rotation correctly")
		{
			CreateAndCompare(root, values, colours);
		}

		alc.deAllocate(root);
		REQUIRE(alc.getCurrentlyAllocated() == 0);
	}

	// The same tree as in balancedInsert section
	createBalancedTree(root, alc);

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
		vector<int> values{ 25, 10, 32, 3, 20, 29, 35, 1, 5, 27, 30 };
		vector<Colour> colours{ Colour::BLACK, Colour::RED, Colour::RED, Colour::BLACK, Colour::BLACK,
								Colour::BLACK, Colour::BLACK, Colour::RED, Colour::RED, Colour::RED, Colour::RED, };

		std::vector<int>::iterator itv = values.begin();
		std::vector<Colour>::iterator itc = colours.begin();

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

		values.erase(itv + 7);
		colours.erase(itc + 7);
		// values: 25, 10, 32, 3, 20, 29, 35, 5, 27, 30
		// colours: B, R, R, B, B, B, B, R, R, R

		SECTION("Removes a red leaf without changing anthing else")
		{
			CreateAndCompare(root, values, colours);
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

		values[0] = 27;
		values[3] = 5;
		values.erase(itv + 7, itv + 9);
		colours.erase(itc + 7, itc + 9);
		// values: 27, 10, 32, 5, 20, 29, 35, 30
		// colours: B, R, R, B, B, B, B, R

		SECTION("Swaps the value with the smallest element which is bigger than the node we want to delete and removes it as it is red")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::erase(root, alc, 27);

		/* After erase:
		*               29B
		*             /     \
		*           10R      32R
		*          /  \      /  \
		*        5B   20B  30B  35B                 
		*/

		values[0] = 29;
		values[5] = 30;
		values.erase(itv + 7);
		colours.erase(itc + 7);
		// values: 29, 10, 32, 5, 20, 30, 35
		// colours: B, R, R, B, B, B, B

		SECTION("Double swap performed")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::erase(root, alc, 5);

		/* After erase:
		*               29B
		*             /     \
		*           10B      32R
		*             \      /  \
		*             20R  30B  35B
		*/

		values.erase(itv + 3);
		colours.erase(itc + 3);
		colours[1] = Colour::BLACK;
		colours[3] = Colour::RED;
		// values: 29, 10, 32, 20, 30, 35
		// colours: B, B, R, R, B, B

		SECTION("Sibling has 2 black children (nulls)")
		{
			CreateAndCompare(root, values, colours);
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

		values.erase(itv + 4, values.end());
		colours.erase(itc + 4, colours.end());
		values[0] = 32;
		colours[0] = Colour::BLACK;
		values[1] = 29;
		colours[1] = Colour::BLACK;
		values[2] = 35;
		colours[2] = Colour::BLACK;
		values[3] = 30;
		colours[3] = Colour::RED;
		// values: 32, 29, 35, 30
		// colours: B, B, B, R

		SECTION("Red sibling case, followed by 2 black children case")
		{
			CreateAndCompare(root, values, colours);
		}

		Operation::erase(root, alc, 35);		

		/* After erase:
		*               30B
		*              /   \
		*            29B    32B
		*/

		values.erase(itv + 2);
		std::swap(values[0], values[2]);
		colours.pop_back();
		// values: 30, 29, 32
		// colours: B, B, B

		SECTION("'Near child is red' case, followed by 'Far child is red' case")
		{
			CreateAndCompare(root, values, colours);
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

TEST_CASE("RedBlackTree", "[RBTree]")
{
	RBT tree;

	emptyTree(tree);

	vector<int> insertInOrder { 4, 3, 2, 10, 5, 1, 0, 7, 11, 15 };
	vector<int> expected { 0, 1, 2, 3, 4, 5, 7, 10, 11, 15 };
	createRBTfromVector(tree, insertInOrder);
	
	REQUIRE(tree.getHeight() == 4);
	REQUIRE(tree.size() == 10);
	REQUIRE(tree.printInOrder() == expected);
	

	REQUIRE(tree.contains(4));
	REQUIRE(tree.contains(10));
	REQUIRE(tree.contains(6) == false);
	REQUIRE(tree.contains(100) == false);


	vector<int> deleteInOrder { 3, 5, 7, 0, 2 };
	vector<int> expected2{ 1, 4, 10, 11, 15 };
	deleteInOrderFromRBT(tree, deleteInOrder);
	REQUIRE(tree.getHeight() == 3);
	REQUIRE(tree.size() == 5);
	REQUIRE(tree.printInOrder() == expected2);

	tree.clear();
	emptyTree(tree);
}