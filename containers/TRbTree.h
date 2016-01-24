/* Copyright 2016 Tom Kim
*
* Implementation of red-black tree as described in Introduction to Algorithms
* by Corman et al.
*/
#pragma once

#include <stdlib.h>

class RbTreeNode
{
public:
	enum Color { RED, BLACK };
	static RbTreeNode* NIL;

	RbTreeNode(void);

	Color m_color;
	RbTreeNode* m_parent;
	RbTreeNode* m_right;
	RbTreeNode* m_left;
	int m_value;
};

class RbTree
{
public:
	RbTree::RbTree(void);
	void insert(int value);

private:
	typedef class RbTreeNode Node;

	Node* m_root;

	void leftRotate(Node* x);
	void rightRotate(Node* x);
	void insert(Node* z);
	void insertFixup(Node* z);
};
