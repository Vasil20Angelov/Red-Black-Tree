#pragma once
#include "Node.h"
#include "Allocator.h"
#include "Rotation.h"
#include "Operations.h"

template <class Alloc = Allocator> 
class RBT {

public:
	RBT();
	RBT(int value);
	~RBT();

	inline void insert(int _value);
	inline void erase(int key);
	inline void clear();

	inline bool contains(int key) const;
	inline bool empty() const;
	inline size_t size() const;
	size_t getHeight() const;

private:
	Node* root;
	Alloc allocator;
};
