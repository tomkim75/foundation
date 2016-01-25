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

void
RbTree::erase(int value)
{
	iterator itr = find(value);

	if (itr != end())
		erase(itr.m_node);
}

void
RbTree::erase(iterator itr)
{
	if (itr != end())
		erase(itr.m_node);
}

RbTree::iterator
RbTree::find(int value) const
{
	Node* node = m_root;

	while (node != Node::NIL)
	{
		if (value < node->m_value)
			node = node->m_left;
		else if (value > node->m_value)
			node = node->m_right;
		else
			return iterator(this, node);
	}

	return iterator(this, NULL);
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

void
RbTree::transplant(Node* u, Node* v)
{
	if (u->m_parent == Node::NIL)
		m_root = v;
	else if (u == u->m_parent->m_left)
		u->m_parent->m_left = v;
	else
		u->m_parent->m_right = v;

	v->m_parent = u->m_parent;
}

RbTree::Node*
RbTree::minimum(Node* x)
{
	while (x->m_left != Node::NIL)
		x = x->m_left;
	return x;
}

void
RbTree::erase(Node* z)
{
	Node* x = NULL;
	Node* y = z;
	Node::Color yOriginalColor = y->m_color;

	if (z->m_left == Node::NIL)
	{
		x = z->m_right;
		transplant(z, z->m_right);
	}
	else if (z->m_right == Node::NIL)
	{
		x = z->m_left;
		transplant(z, z->m_left);
	}
	else
	{
		y = minimum(z->m_right);
		Node::Color yOriginalColor = y->m_color;
		x = y->m_right;

		if (y->m_parent == z)
			x->m_parent = y;
		else
		{
			transplant(y, y->m_right);
			y->m_right = z->m_right;
			y->m_right->m_parent = y;
		}

		transplant(z, y);
		y->m_left = z->m_left;
		y->m_left->m_parent = y;
		y->m_color = z->m_color;
	}

	if (yOriginalColor == Node::BLACK)
	{
		eraseFixup(x);
	}
}

void
RbTree::eraseFixup(Node* x)
{
	while (x != m_root && x->m_color == Node::BLACK)
	{
		if (x == x->m_parent->m_left)
		{
			Node* w = x->m_parent->m_right;

			if (w->m_color == Node::RED)
			{
				w->m_color = Node::BLACK;
				x->m_parent->m_color = Node::RED;
				leftRotate(x->m_parent);
				w = x->m_parent->m_right;
			}

			if (w->m_left->m_color == Node::BLACK && w->m_right->m_color == Node::BLACK)
			{
				w->m_color = Node::RED;
				x = x->m_parent;
			}
			else
			{
				if (w->m_right->m_color == Node::BLACK)
				{
					w->m_left->m_color = Node::BLACK;
					w->m_color = Node::RED;
					rightRotate(w);
					w = x->m_parent->m_right;
				}

				w->m_color = x->m_parent->m_color;
				x->m_parent->m_color = Node::BLACK;
				w->m_right->m_color = Node::BLACK;
				leftRotate(x->m_parent);
				x = m_root;
			}
		}
		else
		{
			Node* w = x->m_parent->m_left;

			if (w->m_color == Node::RED)
			{
				w->m_color = Node::BLACK;
				x->m_parent->m_color = Node::RED;
				rightRotate(x->m_parent);
				w = x->m_parent->m_left;
			}

			if (w->m_right->m_color == Node::BLACK && w->m_left->m_color == Node::BLACK)
			{
				w->m_color = Node::RED;
				x = x->m_parent;
			}
			else
			{
				if (w->m_left->m_color == Node::BLACK)
				{
					w->m_right->m_color = Node::BLACK;
					w->m_color = Node::RED;
					leftRotate(w);
					w = x->m_parent->m_left;
				}

				w->m_color = x->m_parent->m_color;
				x->m_parent->m_color = Node::BLACK;
				w->m_left->m_color = Node::BLACK;
				rightRotate(x->m_parent);
				x = m_root;
			}
		}
	}

	x->m_color = Node::BLACK;
}
