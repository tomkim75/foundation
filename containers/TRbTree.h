/* Copyright 2016 Tom Kim
*
* Implementation of red-black tree as described in Introduction to Algorithms
* by Corman et al.
*/
#pragma once

#include <stdlib.h>
#include <cassert>

template <typename K>
class RbTreeNode
{
	template <typename K> friend class RbTree;
	template <typename K> friend class RbTreeItrBase;
  	template <typename K> friend class RbTreeItr;
	template <typename K> friend class RbTreeConstItr;

private:
	enum Color { RED, BLACK };
	static RbTreeNode* NIL;

	Color m_color;
	RbTreeNode* m_parent;
	RbTreeNode* m_right;
	RbTreeNode* m_left;
	K m_key;

	RbTreeNode(void) : m_color(BLACK), m_parent(NIL), m_right(NIL), m_left(NIL), m_key(0) { }
};

template <typename K>
class RbTreeItrBase
{
protected:
	typedef RbTreeNode<K> Node;
	typedef RbTree<K> Tree;

	Tree* m_tree;
	Node* m_node;

	RbTreeItrBase(Tree* tree, Node* node) : m_tree(tree), m_node(node) { }
	void increment(void);
	void decrement(void);
};

template <typename K>
class RbTreeItr : private RbTreeItrBase<K>
{
	template <typename K> friend class RbTree;
	template <typename K> friend class RbTreeConstItr;

public:
	RbTreeItr(const RbTreeItr& other) : RbTreeItrBase(other.m_tree, other.m_node) { }

	bool operator==(const RbTreeItr& other) const { return m_node == other.m_node; }
	bool operator!=(const RbTreeItr& other) const { return m_node != other.m_node; }
	bool operator==(const RbTreeConstItr<K>& other) const { return m_node == other.m_node; }
	bool operator!=(const RbTreeConstItr<K>& other) const { return m_node != other.m_node; }
	RbTreeItr& operator++(void) { increment(); return *this; }
	RbTreeItr& operator--(void) { decrement(); return *this; }
	K& operator*(void) { return const_cast<Node*>(m_node)->m_key; }

private:
	RbTreeItr(const RbTreeConstItr<K>& other) : RbTreeItrBase(other.m_tree, other.m_node) { } // made private to avoid conversion outside of friends
	RbTreeItr(Tree* tree, Node* node) : RbTreeItrBase(tree, node) { }
};

template <typename K>
class RbTreeConstItr : private RbTreeItrBase<K>
{
	template <typename K> friend class RbTree;
	template <typename K> friend class RbTreeItr;

public:
	RbTreeConstItr(const RbTreeConstItr& other) : RbTreeItrBase(other.m_tree, other.m_node) { }
	RbTreeConstItr(const RbTreeItr<K>& other) : RbTreeItrBase(other.m_tree, other.m_node) { }

	bool operator==(const RbTreeConstItr& other) const { return m_node == other.m_node; }
	bool operator!=(const RbTreeConstItr& other) const { return m_node != other.m_node; }
	bool operator==(const RbTreeItr<K>& other) const { return m_node == other.m_node; }
	bool operator!=(const RbTreeItr<K>& other) const { return m_node != other.m_node; }
	RbTreeConstItr& operator++(void) { increment(); return *this; }
	RbTreeConstItr& operator--(void) { decrement(); return *this; }
	const K& operator*(void) const { return m_node->m_key; }

private:
	RbTreeConstItr(const Tree* tree, Node* node) : RbTreeItrBase(const_cast<Tree*>(tree), node) { }
};

template <typename K>
class RbTree
{
	typedef class RbTreeNode<K> Node;

public:
	typedef RbTreeItr<K> Iterator;
	typedef RbTreeConstItr<K> ConstIterator;

	RbTree(void);

	void insert(const K& key);
	void erase(const K& key);
	void erase(const ConstIterator& itr);

	Iterator find(const K& key) { return Iterator(const_cast<const RbTree*>(this)->find(key)); }
	Iterator begin(void) { return Iterator(const_cast<const RbTree*>(this)->begin()); }
	Iterator end(void) { return Iterator(const_cast<const RbTree*>(this)->begin()); }
	Iterator last(void) { return Iterator(const_cast<const RbTree*>(this)->last()); }

	ConstIterator find(const K& key) const;
	ConstIterator begin(void) const;
	ConstIterator end(void) const;
	ConstIterator last(void) const;

private:
	Node* m_root;

	void leftRotate(Node* x);
	void rightRotate(Node* x);
	void insert(Node* z);
	void insertFixup(Node* z);

	void transplant(Node* u, Node* v);
	Node* minimum(Node* x);
	void erase(Node* v);
	void eraseFixup(Node* x);
};

// RbTreeNode
//
template <typename K>
RbTreeNode<K>* RbTreeNode<K>::NIL = new RbTreeNode();

// RbTreeItrBase
//
template <typename K>
void
RbTreeItrBase<K>::increment(void)
{
	assert(m_node != NULL);

	if (m_node->m_right != Node::NIL)
	{
		m_node = m_node->m_right;

		while (m_node->m_left != Node::NIL)
			m_node = m_node->m_left;

		return;
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

	return;
}

template <typename K>
void
RbTreeItrBase<K>::decrement(void)
{
	assert(m_node != NULL);

	if (m_node->m_left != Node::NIL)
	{
		m_node = m_node->m_left;

		while (m_node->m_right != Node::NIL)
			m_node = m_node->m_right;

		return;
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

	return;
}

// RbTree
//
template <typename K>
RbTree<K>::RbTree(void)
{
	m_root = Node::NIL;
}

template <typename K>
typename RbTree<K>::ConstIterator
RbTree<K>::begin(void) const
{
	Node* node = m_root;
	if (m_root == Node::NIL) return end();
	while (node->m_left != Node::NIL) node = node->m_left;
	return ConstIterator(this, node);
}

template <typename K>
typename RbTree<K>::ConstIterator
RbTree<K>::end(void) const
{
	return ConstIterator(this, NULL);
}

template <typename K>
typename RbTree<K>::ConstIterator
RbTree<K>::last(void) const
{
	Node* node = m_root;
	if (m_root == Node::NIL) return end();
	while (node->m_right != Node::NIL) node = node->m_right;
	return ConstIterator(this, node);
}

template <typename K>
void
RbTree<K>::insert(const K& key)
{
	Node* node = new Node();
	node->m_color = Node::RED;
	node->m_key = key;
	insert(node);
}

template <typename K>
void
RbTree<K>::erase(const K& key)
{
	Iterator itr = find(key);

	if (itr != end())
		erase(const_cast<Node*>(itr.m_node));
}

template <typename K>
void
RbTree<K>::erase(const ConstIterator& itr)
{
	if (itr != end())
		erase(const_cast<Node*>(itr.m_node));
}

template <typename K>
typename RbTree<K>::ConstIterator
RbTree<K>::find(const K& key) const
{
	Node* node = m_root;

	while (node != Node::NIL)
	{
		if (key < node->m_key)
			node = node->m_left;
		else if (key > node->m_key)
			node = node->m_right;
		else
			return ConstIterator(this, node);
	}

	return ConstIterator(this, NULL);
}

template <typename K>
inline void
RbTree<K>::leftRotate(Node* x)
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

template <typename K>
inline void
RbTree<K>::rightRotate(Node* x)
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

template <typename K>
inline void
RbTree<K>::insert(Node* z)
{
	Node* y = Node::NIL;
	Node* x = m_root;

	while (x != Node::NIL)
	{
		y = x;

		if (z->m_key < x->m_key)
			x = x->m_left;
		else
			x = x->m_right;
	}

	z->m_parent = y;

	if (y == Node::NIL)
		m_root = z;
	else if (z->m_key < y->m_key)
		y->m_left = z;
	else
		y->m_right = z;

	z->m_left = Node::NIL;
	z->m_right = Node::NIL;
	z->m_color = Node::RED;

	insertFixup(z);
}

template <typename K>
inline void
RbTree<K>::insertFixup(Node* z)
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

template <typename K>
void
RbTree<K>::transplant(Node* u, Node* v)
{
	if (u->m_parent == Node::NIL)
		m_root = v;
	else if (u == u->m_parent->m_left)
		u->m_parent->m_left = v;
	else
		u->m_parent->m_right = v;

	v->m_parent = u->m_parent;
}

template <typename K>
typename RbTree<K>::Node*
RbTree<K>::minimum(Node* x)
{
	while (x->m_left != Node::NIL)
		x = x->m_left;
	return x;
}

template <typename K>
void
RbTree<K>::erase(Node* z)
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

template <typename K>
void
RbTree<K>::eraseFixup(Node* x)
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
