#pragma once
#include "Node.h"
#include <set>
class RBT {
public:
	RBT();
	RBT(int value);
	~RBT();

	bool leaf() const;
	int value() const;

private:
	Node* root;
};
