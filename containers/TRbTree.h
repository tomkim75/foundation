/* Copyright 2016 Tom Kim
*
* Implementation of red-black tree as described in Introduction to Algorithms
* by Corman et al.
*/
#pragma once

#include <stdlib.h>

class RbTreeNode
{
	friend class RbTree;
	friend class RbTreeItr;

private:
	enum Color { RED, BLACK };
	static RbTreeNode* NIL;

	RbTreeNode(void);

	Color m_color;
	RbTreeNode* m_parent;
	RbTreeNode* m_right;
	RbTreeNode* m_left;
	int m_value;
};

class RbTreeItr
{
	friend class RbTree;
	
public:
	RbTreeItr(const RbTreeItr& other);

	bool operator==(const RbTreeItr& other) const;
	bool operator!=(const RbTreeItr& other) const;
	int operator*(void) const;
	RbTreeItr operator++(void);	
	RbTreeItr operator--(void);

private:
	typedef RbTreeNode Node;

	RbTreeItr(const RbTree* tree, Node* node);

	const RbTree* m_tree;
	RbTreeNode* m_node;
};

class RbTree
{
public:
	typedef RbTreeItr iterator;

	RbTree::RbTree(void);
	void insert(int value);
	iterator begin(void) const;
	iterator end(void) const;
	iterator last(void) const;

private:
	typedef class RbTreeNode Node;

	Node* m_root;

	void leftRotate(Node* x);
	void rightRotate(Node* x);
	void insert(Node* z);
	void insertFixup(Node* z);
};
