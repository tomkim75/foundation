/*
Copyright 2016 Tom Kim
Implementation of a deque container with an STL-like interface.
*/
#pragma once

template <typename V> class TDeque;

template <typename V>
class TDequeItr
{
    typedef TDeque<V> Vector;
    template <typename V> friend class TDeque;
    template <typename K> friend class TDequeConstItr;

public:

    TDequeItr(const TDequeItr& itr) : m_deque(itr.m_deque), m_pos(itr.m_pos) { }

    bool operator==(const TDequeItr& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TDequeItr& other) const { return m_pos != other.m_pos; }
    bool operator==(const TDequeConstItr<V>& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TDequeConstItr<V>& other) const { return m_pos != other.m_pos; }
    TDequeItr& operator++(void) { assert(m_deque->m_size != -1); m_pos++; if (m_pos == m_deque->m_size) m_pos = -1; return *this; }
    TDequeItr& operator--(void) { assert(m_deque->m_size != -1); m_pos--; return *this; }
    V& operator*(void) { return m_deque->m_array[m_pos]; }

private:

    TDequeItr(Vector* deque, size_t pos) : m_deque(deque), m_pos(pos) { }

    Vector* m_deque;
    size_t m_pos;
};

template <typename V>
class TDequeConstItr
{
    typedef TDeque<V> Vector;
    template <typename V> friend class TDeque;
    template <typename V> friend class TDequeItr;

public:

    TDequeConstItr(const TDequeConstItr& itr) : m_deque(itr.m_deque), m_pos(itr.m_pos) { }
    TDequeConstItr(const TDequeItr<V>& itr) : m_deque(itr.m_deque), m_pos(itr.m_pos) { }

    bool operator==(const TDequeConstItr& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TDequeConstItr& other) const { return m_pos != other.m_pos; }
    bool operator==(const TDequeItr<V>& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TDequeItr<V>& other) const { return m_pos != other.m_pos; }
    TDequeConstItr& operator++(void) { assert(m_deque->m_size != -1); m_pos++; if (m_pos == m_deque->m_size) m_pos = -1; return *this; }
    TDequeConstItr& operator--(void) { assert(m_deque->m_size != -1); m_pos--; return *this; }
    const V& operator*(void) const { return m_deque->m_array[m_pos]; }

private:

    TDequeConstItr(Vector* deque, size_t pos) : m_deque(deque), m_pos(pos) { }

    Vector* m_deque;
    size_t m_pos;
};


template <typename V>
class TDeque
{
    typedef TDeque<V> Node;
    template <typename K> friend class TDequeItr;
    template <typename K> friend class TDequeConstItr;

public:

    typedef TDequeItr<V> iterator;
    typedef TDequeConstItr<V> const_iterator;

    TDeque(void);
    TDeque(size_t capacity);

    void push_back(const V& value);
    void pop_back(void);

    V& back(void);
    const V& back(void) const;

    iterator begin(void) { return iterator(this, 0); }
    iterator last(void) { return iterator(this, m_size - 1); }
    iterator end(void) { return iterator(this, -1); }

    const_iterator begin(void) const { return const_iterator(this, 0); }
    const_iterator last(void) const { return const_iterator(this, m_size - 1); }
    const_iterator end(void) const { return const_iterator(this, -1); }

    V* buf(void) const { return m_array; }
    size_t size(void) const { return m_size; }

private:

    void grow(size_t capacity);

    V* m_array;
    size_t m_capacity;
    size_t m_size;
};

template <typename V>
TDeque<V>::TDeque(void)
    : m_array(NULL), m_capacity(0), m_size(0)
{ }

template <typename V>
TDeque<V>::TDeque(size_t capacity)
    : m_array(NULL), m_capacity(capacity), m_size(0)
{
    grow(capacity);
}

template <typename V>
void
TDeque<V>::grow(size_t capacity)
{
    size_t capacityBytes = sizeof(V) * capacity;
    V* newArray = static_cast<V*>(operator new(capacityBytes));

    if (newArray == NULL)
        throw std::bad_alloc();

    for (size_t i = 0; i < m_size; i++)
    {
        V& v = m_array[i];
        new (newArray + i) V(v);
    }

    delete[] m_array;
    m_array = newArray;
    m_capacity = capacity;
}

template <typename V>
void
TDeque<V>::push_back(const V& value)
{
    if (m_capacity == 0)
        grow(1);
    else if (m_size + 1 > m_capacity)
        grow(m_capacity * 2);

    V* v = new (m_array + m_size) V(value);
    m_size++;
}

template <typename V>
void
TDeque<V>::pop_back(void)
{
    assert(m_size != 0);
    V& v = m_array[m_size - 1];
    v.~V();
    m_size--;
}

template <typename V>
V&
TDeque<V>::back(void)
{
    assert(m_size > 0);
    return m_array[m_size - 1];
}

template <typename V>
const V&
TDeque<V>::back(void) const
{
    assert(m_size > 0);
    return m_array[m_size - 1];
}
