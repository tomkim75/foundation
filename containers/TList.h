#pragma once

template <typename V>
class TListNode
{
    template <typename K> friend class TList;
    template <typename K> friend class TListItr;
    //template <typename K> friend class TListConstItr;

public:

    TListNode(const V& value) : m_value(value), m_prev(NULL), m_next(NULL) { }

private:

    TListNode* m_prev;
    TListNode* m_next;
    V m_value;
};

template <typename V>
class TListItr
{
    typedef TListNode<V> Node;

public:

    TListItr(Node* node) : m_node(node) { }

    TListItr& operator++(void) { assert(m_node != NULL); m_node = m_node->m_next; return *this; }
    bool operator==(const TListItr& other) const { return m_node == other.m_node; }
    bool operator!=(const TListItr& other) const { return m_node != other.m_node; }
    V& operator*(void) { return m_node->m_value; }

private:

    Node* m_node;
};

template <typename V>
class TList
{
    typedef TListNode<V> Node;

public:

    typedef TListItr<V> iterator;
    //typedef TListConstItr<V> const_iterator;

    TList(void);

    void push_front(const V& value);
    void push_back(const V& value);

    // Return is void because return of V is inefficient requiring V to be
    // copied to lvalue.
    //
    void pop_front(void);
    void pop_back(void);

    V& front(void);
    V& back(void);

    const V& front(void) const;
    const V& back(void) const;

    iterator begin(void) { return iterator(m_head); }
    iterator last(void) { return iterator(m_tail); }
    iterator end(void) { return iterator(NULL); }

    //const_iterator begin(void) const;
    //const_iterator end(void) const;

protected:

    Node* m_head;
    Node* m_tail;
};

template <typename V>
TList<V>::TList(void)
    : m_head(NULL), m_tail(NULL)
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

    return value;
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
