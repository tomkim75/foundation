/* Copyright 2016 Tom Kim
 *
 * Implementation of red-black tree as described in Introduction to Algorithms
 * by Corman et al.
 */
#pragma once

#include "TRbTree.h"
#include <cassert>

// RbTreeNode
//
RbTreeNode* RbTreeNode::NIL = new RbTreeNode();

RbTreeNode::RbTreeNode(void)
	: m_color(BLACK), m_parent(NIL), m_right(NIL), m_left(NIL), m_value(0)
{ }

// RbTreeItr
//
RbTreeItr::RbTreeItr(const RbTree* tree, Node* node)
	: m_tree(tree), m_node(node)
{ }

RbTreeItr::RbTreeItr(const RbTreeItr& other)
	: m_tree(other.m_tree), m_node(other.m_node)
{ }

bool
RbTreeItr::operator==(const RbTreeItr& other) const
{
	return m_node == other.m_node;
}

bool
RbTreeItr::operator!=(const RbTreeItr& other) const
{
	return m_node != other.m_node;
}


int
RbTreeItr::operator*(void) const
{
	return m_node->m_value;
}

RbTreeItr
RbTreeItr::operator++(void)
{
	assert(m_node != NULL);

	if (m_node->m_right != Node::NIL)
	{
		m_node = m_node->m_right;

		while (m_node->m_left != Node::NIL)
			m_node = m_node->m_left;

		return *this;
	}

	while (1)
	{
		Node* m_old = m_node;
		m_node = m_node->m_parent;

		if (m_node == Node::NIL || m_node->m_left == m_old)
			break;
	}

	if (m_node == Node::NIL)
		m_node = NULL;

	return *this;
}

RbTreeItr
RbTreeItr::operator--(void)
{
	assert(m_node != NULL);

	if (m_node->m_left != Node::NIL)
	{
		m_node = m_node->m_left;

		while (m_node->m_right != Node::NIL)
			m_node = m_node->m_right;

		return *this;
	}

	while (1)
	{
		Node* m_old = m_node;
		m_node = m_node->m_parent;

		if (m_node == Node::NIL || m_node->m_right == m_old)
			break;
	}

	if (m_node == Node::NIL)
		m_node = NULL;

	return *this;
}

// RbTree
//
RbTree::RbTree(void)
{
	m_root = Node::NIL;
}

RbTree::iterator
RbTree::begin(void) const
{
	Node* node = m_root;
	if (m_root == Node::NIL) return end();
	while (node->m_left != Node::NIL) node = node->m_left;
	return iterator(this, node);
}

RbTree::iterator
RbTree::end(void) const
{
	return iterator(this, NULL);
}

RbTree::iterator
RbTree::last(void) const
{
	Node* node = m_root;
	if (m_root == Node::NIL) return end();
	while (node->m_right != Node::NIL) node = node->m_right;
	return iterator(this, node);
}

void
RbTree::insert(int value)
{
	Node* node = new Node();
	node->m_color = Node::RED;
	node->m_value = value;
	insert(node);
}

inline void
RbTree::leftRotate(Node* x)
{
	Node* y = x->m_right;
	x->m_right = y->m_left;

	if (y->m_left != Node::NIL)
		y->m_left->m_parent = x;

	y->m_parent = x->m_parent;

	if (x->m_parent == Node::NIL)
		m_root = y;
	else if (x == x->m_parent->m_left)
		x->m_parent->m_left = y;
	else
		x->m_parent->m_right = y;

	y->m_left = x;
	x->m_parent = y;
}

inline void
RbTree::rightRotate(Node* x)
{
	Node* y = x->m_left;
	x->m_left = y->m_right;

	if (y->m_right != Node::NIL)
		y->m_right->m_parent = x;

	y->m_parent = x->m_parent;

	if (x->m_parent == Node::NIL)
		m_root = y;
	else if (x == x->m_parent->m_right)
		x->m_parent->m_right = y;
	else
		x->m_parent->m_left = y;

	y->m_right = x;
	x->m_parent = y;
}

inline void
RbTree::insert(Node* z)
{
	Node* y = Node::NIL;
	Node* x = m_root;

	while (x != Node::NIL)
	{
		y = x;

		if (z->m_value < x->m_value)
			x = x->m_left;
		else
			x = x->m_right;
	}

	z->m_parent = y;

	if (y == Node::NIL)
		m_root = z;
	else if (z->m_value < y->m_value)
		y->m_left = z;
	else
		y->m_right = z;

	z->m_left = Node::NIL;
	z->m_right = Node::NIL;
	z->m_color = Node::RED;

	insertFixup(z);
}

inline void
RbTree::insertFixup(Node* z)
{
	while (z->m_parent->m_color == Node::RED)
	{
		if (z->m_parent == z->m_parent->m_parent->m_left)
		{
			Node* y = z->m_parent->m_parent->m_right;

			if (y->m_color == Node::RED)
			{
				z->m_parent->m_color = Node::BLACK;
				y->m_color = Node::BLACK;
				z->m_parent->m_parent->m_color = Node::RED;
				z = z->m_parent->m_parent;
			}
			else
			{
				if (z == z->m_parent->m_right)
				{
					z = z->m_parent;
					leftRotate(z);
				}

				z->m_parent->m_color = Node::BLACK;
				z->m_parent->m_parent->m_color = Node::RED;
				rightRotate(z->m_parent->m_parent);
			}
		}
		else
		{
			Node* y = z->m_parent->m_parent->m_left;

			if (y->m_color == Node::RED)
			{
				z->m_parent->m_color = Node::BLACK;
				y->m_color = Node::BLACK;
				z->m_parent->m_parent->m_color = Node::RED;
				z = z->m_parent->m_parent;
			}
			else
			{
				if (z == z->m_parent->m_left)
				{
					z = z->m_parent;
					rightRotate(z);
				}

				z->m_parent->m_color = Node::BLACK;
				z->m_parent->m_parent->m_color = Node::RED;
				leftRotate(z->m_parent->m_parent);
			}
		}
	}

	m_root->m_color = Node::BLACK;
}
