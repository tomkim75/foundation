/* Copyright 2016 Tom Kim
*
* Implementation of red-black tree as described in Introduction to Algorithms
* by Corman et al.
*/
#pragma once

#include <stdlib.h>
#include <cassert>

template <typename K>
class TRbTreeNode
{
	template <typename K> friend class TRbTree;
	template <typename K> friend class TRbTreeItrBase;
  	template <typename K> friend class TRbTreeItr;
	template <typename K> friend class TRbTreeConstItr;

private:

	enum Color { RED, BLACK };
	static TRbTreeNode* NIL;

	Color m_color;
	TRbTreeNode* m_parent;
	TRbTreeNode* m_right;
	TRbTreeNode* m_left;
	K m_key;

	TRbTreeNode(void) : m_color(BLACK), m_parent(NIL), m_right(NIL), m_left(NIL), m_key(0) { }
};

template <typename K>
class TRbTreeItrBase
{
protected:

	typedef TRbTreeNode<K> Node;
	typedef TRbTree<K> Tree;

	Tree* m_tree;
	Node* m_node;

	TRbTreeItrBase(Tree* tree, Node* node) : m_tree(tree), m_node(node) { }
	void increment(void);
	void decrement(void);
};

template <typename K>
class TRbTreeItr : private TRbTreeItrBase<K>
{
	template <typename K> friend class TRbTree;
	template <typename K> friend class TRbTreeConstItr;

public:

	TRbTreeItr(const TRbTreeItr& other) : TRbTreeItrBase(other.m_tree, other.m_node) { }

	bool operator==(const TRbTreeItr& other) const { return m_node == other.m_node; }
	bool operator!=(const TRbTreeItr& other) const { return m_node != other.m_node; }
	bool operator==(const TRbTreeConstItr<K>& other) const { return m_node == other.m_node; }
	bool operator!=(const TRbTreeConstItr<K>& other) const { return m_node != other.m_node; }
	TRbTreeItr& operator++(void) { increment(); return *this; }
	TRbTreeItr& operator--(void) { decrement(); return *this; }
	K& operator*(void) { return const_cast<Node*>(m_node)->m_key; }

private:

	TRbTreeItr(const TRbTreeConstItr<K>& other) : TRbTreeItrBase(other.m_tree, other.m_node) { } // made private to avoid conversion outside of friends
	TRbTreeItr(Tree* tree, Node* node) : TRbTreeItrBase(tree, node) { }
};

template <typename K>
class TRbTreeConstItr : private TRbTreeItrBase<K>
{
	template <typename K> friend class TRbTree;
	template <typename K> friend class TRbTreeItr;

public:

	TRbTreeConstItr(const TRbTreeConstItr& other) : TRbTreeItrBase(other.m_tree, other.m_node) { }
	TRbTreeConstItr(const TRbTreeItr<K>& other) : TRbTreeItrBase(other.m_tree, other.m_node) { }

	bool operator==(const TRbTreeConstItr& other) const { return m_node == other.m_node; }
	bool operator!=(const TRbTreeConstItr& other) const { return m_node != other.m_node; }
	bool operator==(const TRbTreeItr<K>& other) const { return m_node == other.m_node; }
	bool operator!=(const TRbTreeItr<K>& other) const { return m_node != other.m_node; }
	TRbTreeConstItr& operator++(void) { increment(); return *this; }
	TRbTreeConstItr& operator--(void) { decrement(); return *this; }
	const K& operator*(void) const { return m_node->m_key; }

private:

	TRbTreeConstItr(const Tree* tree, Node* node) : TRbTreeItrBase(const_cast<Tree*>(tree), node) { }
};

template <typename K>
class TRbTree
{
	typedef class TRbTreeNode<K> Node;

public:

	typedef TRbTreeItr<K> Iterator;
	typedef TRbTreeConstItr<K> ConstIterator;

	TRbTree(void);

	void insert(const K& key);
	void erase(const K& key);
	void erase(Iterator itr);

	Iterator find(const K& key) { return Iterator(const_cast<const TRbTree*>(this)->find(key)); }
	Iterator begin(void) { return Iterator(this, m_first); }
	Iterator end(void) { return Iterator(this, NULL); }
	Iterator last(void) { return Iterator(this, m_last); }

	ConstIterator find(const K& key) const;
	ConstIterator begin(void) const { return ConstIterator(this, m_first); }
	ConstIterator end(void) const { return ConstIterator(this, NULL); }
	ConstIterator last(void) const { return ConstIterator(this, m_last); }

	size_t size(void) const { return m_size; }
	size_t maxDepth(void) const;

private:

	Node* m_root;
	Node* m_first;
	Node* m_last;
	size_t m_size;

	void leftRotate(Node* x);
	void rightRotate(Node* x);
	void insert(Node* z);
	void insertFixup(Node* z);

	void transplant(Node* u, Node* v);
	Node* minimum(Node* x);
	void erase(Node* v);
	void eraseFixup(Node* x);

	size_t maxDepth(Node* node, size_t depth) const;
};

// TRbTreeNode
//
template <typename K>
TRbTreeNode<K>* TRbTreeNode<K>::NIL = new TRbTreeNode();

// TRbTreeItrBase
//
template <typename K>
void
TRbTreeItrBase<K>::increment(void)
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
TRbTreeItrBase<K>::decrement(void)
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

// TRbTree
//
template <typename K>
TRbTree<K>::TRbTree(void)
{
	m_root = Node::NIL;
	m_first = NULL;
	m_last = NULL;
	m_size = 0;
}

template <typename K>
void
TRbTree<K>::insert(const K& key)
{
	Node* node = new Node();
	node->m_key = key;

	insert(node);

	// fix first and last
	if (m_first == NULL || m_last == NULL)
	{
		m_first = node;
		m_last = node;
	}
	else
	{
		if (node->m_key < m_first->m_key)
			m_first = node;
		else if (node->m_key > m_last->m_key)
			m_last = node;
	}
}

template <typename K>
void
TRbTree<K>::erase(const K& key)
{
	Iterator itr = find(key);
	eraes(itr);
}

template <typename K>
void
TRbTree<K>::erase(Iterator itr)
{
	if (itr != end())
	{
		Node* eraseNode = itr.m_node;

		// fix first and last
		if (m_first == m_last)
		{
			m_first = NULL;
			m_last = NULL;
		}
		else
		{
			if (eraseNode == m_first)
				m_first = (++itr).m_node;
			else if (eraseNode == m_last)
				m_last = (--itr).m_node;
		}

		erase(eraseNode);
	}
}

template <typename K>
typename TRbTree<K>::ConstIterator
TRbTree<K>::find(const K& key) const
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
TRbTree<K>::leftRotate(Node* x)
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
TRbTree<K>::rightRotate(Node* x)
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
TRbTree<K>::insert(Node* z)
{
	Node* y = Node::NIL;
	Node* x = m_root;

	while (x != Node::NIL)
	{
		y = x;

		if (z->m_key < x->m_key)
			x = x->m_left;
		else if (z->m_key > x->m_key)
			x = x->m_right;
		else
		{
			x->m_key = z->m_key;
			return;
		}
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

	m_size++;
}

template <typename K>
inline void
TRbTree<K>::insertFixup(Node* z)
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
TRbTree<K>::transplant(Node* u, Node* v)
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
typename TRbTree<K>::Node*
TRbTree<K>::minimum(Node* x)
{
	while (x->m_left != Node::NIL)
		x = x->m_left;
	return x;
}

template <typename K>
void
TRbTree<K>::erase(Node* z)
{
	assert(z != NULL && z != Node::NIL);

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
		yOriginalColor = y->m_color;
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
		eraseFixup(x);
	
	delete z;
	m_size--;
}

template <typename K>
void
TRbTree<K>::eraseFixup(Node* x)
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

template <typename K>
size_t
TRbTree<K>::maxDepth(void) const
{
	if (m_root == Node::NIL)
		return 0;

	size_t depth = 1;
	size_t leftDepth = maxDepth(m_root->m_left, depth);
	size_t rightDepth = maxDepth(m_root->m_right, depth);
	return (leftDepth > rightDepth) ? leftDepth : rightDepth;
}

template <typename K>
size_t
TRbTree<K>::maxDepth(Node* node, size_t depth) const
{
	if (node == Node::NIL)
		return depth;

	depth++;
	size_t leftDepth = maxDepth(node->m_left, depth);
	size_t rightDepth = maxDepth(node->m_right, depth);
	return (leftDepth > rightDepth) ? leftDepth : rightDepth;
}
