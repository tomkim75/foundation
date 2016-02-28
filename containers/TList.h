/*
Copyright 2016 Tom Kim
Implementation of a linked-list container with an STL-like interface.
*/
#pragma once

template <typename V>
class TListNode
{
    template <typename K> friend class TList;
    template <typename K> friend class TListItr;
    template <typename K> friend class TListConstItr;

private:

    TListNode(const V& value) : m_value(value), m_prev(NULL), m_next(NULL) { }

    TListNode* m_prev;
    TListNode* m_next;
    V m_value;
};

template <typename V>
class TListItr
{
    typedef TListNode<V> Node;
    template <typename K> friend class TList;
    template <typename K> friend class TListConstItr;

public:

    TListItr(const TListItr& itr) : m_node(itr.m_node) { }

    bool operator==(const TListItr& other) const { return m_node == other.m_node; }
    bool operator!=(const TListItr& other) const { return m_node != other.m_node; }
    bool operator==(const TListConstItr<V>& other) const { return m_node == other.m_node; }
    bool operator!=(const TListConstItr<V>& other) const { return m_node != other.m_node; }
    TListItr& operator++(void) { assert(m_node != NULL); m_node = m_node->m_next; return *this; }
    TListItr& operator--(void) { assert(m_node != NULL); m_node = m_node->m_prev; return *this; }
    V& operator*(void) { return m_node->m_value; }

private:

    TListItr(Node* node) : m_node(node) { }

    Node* m_node;
};

template <typename V>
class TListConstItr
{
    typedef TListNode<V> Node;
    template <typename K> friend class TList;
    template <typename K> friend class TListItr;

public:

    TListConstItr(const TListConstItr& other) : m_node(other.m_node) { }
    TListConstItr(const TListItr<V>& other) : m_node(other.m_node) { }

    bool operator==(const TListConstItr& other) const { return m_node == other.m_node; }
    bool operator!=(const TListConstItr& other) const { return m_node != other.m_node; }
    bool operator==(const TListItr<V>& other) const { return m_node == other.m_node; }
    bool operator!=(const TListItr<V>& other) const { return m_node != other.m_node; }
    TListConstItr& operator++(void) { assert(m_node != NULL); m_node = m_node->m_next; return *this; }
    TListConstItr& operator--(void) { assert(m_node != NULL); m_node = m_node->m_prev; return *this; }
    const V& operator*(void) const { return m_node->m_value; }

private:

    TListConstItr(Node* node) : m_node(node) { }

    Node* m_node;
};

template <typename V>
class TList
{
    typedef TListNode<V> Node;

public:

    typedef TListItr<V> iterator;
    typedef TListConstItr<V> const_iterator;

    TList(void);

    void push_front(const V& value);
    void push_back(const V& value);
    void push_after(iterator itr, const V& value);
    void push_before(iterator itr, const V& value);

    // Return is void because return of V is inefficient requiring V to be
    // copied to lvalue.
    //
    void pop_front(void);
    void pop_back(void);
    void pop_at(iterator itr);

    V& front(void);
    V& back(void);

    const V& front(void) const;
    const V& back(void) const;

    iterator begin(void) { return iterator(m_head); }
    iterator last(void) { return iterator(m_tail); }
    iterator end(void) { return iterator(NULL); }

    const_iterator begin(void) const { return iterator(m_head); }
    const_iterator last(void) const { return iterator(m_tail); }
    const_iterator end(void) const { return iterator(NULL); }

    size_t size(void) const { return m_size; }

protected:

    Node* m_head;
    Node* m_tail;
    size_t m_size;
};

template <typename V>
TList<V>::TList(void)
    : m_head(NULL), m_tail(NULL), m_size(0)
{ }

template <typename V>
void
TList<V>::push_front(const V& value)
{
    if (m_head == NULL)
    {
        assert(m_tail == NULL);
        m_head = new Node(value);
        m_tail = m_head;
    }
    else
    {
        Node* newNode = new Node(value);
        newNode->m_next = m_head;
        m_head->m_prev = newNode;
        m_head = newNode;
    }

    m_size++;
}

template <typename V>
void
TList<V>::push_back(const V& value)
{
    if (m_tail == NULL)
    {
        assert(m_head == NULL);
        m_tail = new Node(value);
        m_head = m_tail;
    }
    else
    {
        Node* newNode = new Node(value);
        newNode->m_prev = m_tail;
        m_tail->m_next = newNode;
        m_tail = newNode;
    }

    m_size++;
}

template <typename V>
void
TList<V>::push_after(iterator itr, const V& value)
{
    assert(itr.m_node != NULL);

    Node* newNode = new Node(value);
    Node* nextNode = itr.m_node->m_next;

    newNode->m_prev = itr.m_node;
    newNode->m_next = nextNode;
    itr.m_node->m_next = newNode;

    if (nextNode != NULL)
        nextNode->m_prev = newNode;

    if (m_tail == itr.m_node)
        m_tail = newNode;

    m_size++;
}

template <typename V>
void
TList<V>::push_before(iterator itr, const V& value)
{
    assert(itr.m_node != NULL);

    Node* newNode = new Node(value);
    Node* prevNode = itr.m_node->m_prev;

    newNode->m_next = itr.m_node;
    newNode->m_prev = prevNode;
    itr.m_node->m_prev = newNode;

    if (prevNode != NULL)
        prevNode->m_next = newNode;

    if (m_head == itr.m_node)
        m_head = newNode;

    m_size++;
}

template <typename V>
void
TList<V>::pop_front(void)
{
    assert(m_head != NULL);

    Node* deleteNode = m_head;
    m_head = m_head->m_next;

    if (m_head == NULL)
        m_tail = NULL;
    else
        m_head->m_prev = NULL;

    delete deleteNode;
    m_size--;
}

template <typename V>
void
TList<V>::pop_back(void)
{
    assert(m_tail != NULL);

    Node* deleteNode = m_tail;
    m_tail = m_tail->m_prev;

    if (m_tail == NULL)
        m_head = NULL;
    else
        m_tail->m_next = NULL;

    delete deleteNode;
    m_size--;
}

template <typename V>
void
TList<V>::pop_at(iterator itr)
{
    assert(itr != end());

    Node* deleteNode = itr.m_node;
    Node* prevNode = itr.m_node->m_prev;
    Node* nextNode = itr.m_node->m_next;

    if (prevNode != NULL)
        prevNode->m_next = nextNode;

    if (nextNode != NULL)
        nextNode->m_prev = prevNode;
    
    if (itr.m_node == m_tail)
        m_tail = prevNode;

    if (itr.m_node == m_head)
        m_head = nextNode;

    delete deleteNode;
    m_size--;
}

template <typename V>
V&
TList<V>::front(void)
{
    assert(m_head != NULL);
    return m_head->m_value;
}

template <typename V>
V&
TList<V>::back(void)
{
    assert(m_tail != NULL);
    return m_tail->m_value;
}

template <typename V>
const V&
TList<V>::front(void) const
{
    assert(m_head != NULL);
    return m_head->m_value;
}

template <typename V>
const V&
TList<V>::back(void) const
{
    assert(m_tail != NULL);
    return m_tail->m_value;
}
