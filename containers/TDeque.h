/*
Copyright 2016 Tom Kim
Implementation of a deque container with an STL-like interface.
*/
#pragma once

#include "TDequeItr.h"

template <typename V>
class TDeque
{
    typedef TDeque<V> Node;
    template <typename V> friend class TDequeItrBase;
    template <typename V> friend class TDequeItr;
    template <typename V> friend class TDequeConstItr;

public:

    typedef TDequeItr<V> iterator;
    typedef TDequeConstItr<V> const_iterator;

    TDeque(void);
    TDeque(size_t capacity);

    void push_front(const V& value);
    void push_back(const V& value);
    void pop_front(void);
    void pop_back(void);

    V& front(void);
    const V& front(void) const;
    V& back(void);
    const V& back(void) const;

    iterator begin(void) { return iterator::begin(this); }
    iterator last(void) { return iterator::last(this); }
    iterator end(void) { return iterator::end(this); }

    const_iterator begin(void) const { return const_iterator::begin(this); }
    const_iterator last(void) const { return const_iterator::last(this); }
    const_iterator end(void) const { return const_iterator::end(this); }

    V* buf(void) const { return m_array; }
    size_t size(void) const { return m_size; }

private:

    void grow(size_t capacity);
    bool inBounds(size_t pos) const;

    V* m_array;
    size_t m_capacity;
    size_t m_size;
    size_t m_begin;
    size_t m_last;
};

template <typename V>
TDeque<V>::TDeque(void)
    : m_array(NULL), m_capacity(0), m_size(0), m_begin(-1), m_last(-1)
{ }

template <typename V>
TDeque<V>::TDeque(size_t capacity)
    : m_array(NULL), m_capacity(capacity), m_size(0), m_begin(-1), m_last(-1)
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

    for (size_t i = 0, ii = m_begin, iii = m_begin; i < m_size; i++)
    {
        V& v = m_array[ii];
        new (newArray + iii) V(v);

        ii++; iii++;
        if (ii == m_capacity)
            ii = 0;
    }

    delete[] m_array;
    m_array = newArray;
    m_capacity = capacity;
    m_last = m_begin + m_size - 1;
}

template <typename V>
bool
TDeque<V>::inBounds(size_t pos) const
{
    if (pos == -1)
        return false;
    else if (m_begin <= m_last)
        return pos >= m_begin && pos <= m_last;
    else
        return (pos >= m_begin && pos < m_capacity) || (pos >= 0 && pos <= m_last);
}

template <typename V>
void
TDeque<V>::push_front(const V& value)
{
    if (m_capacity == 0)
        grow(1);
    else if (m_size + 1 > m_capacity)
        grow(m_capacity * 2);

    // first insert
    if (m_size == 0)
    {
        m_begin = 0;
        m_last = 0;
    }
    else
    {
        m_begin--;
        if (m_begin == -1) m_begin = m_capacity - 1;
    }

    V* v = new (m_array + m_begin) V(value);
    m_size++;
}

template <typename V>
void
TDeque<V>::push_back(const V& value)
{
    if (m_capacity == 0)
        grow(1);
    else if (m_size + 1 > m_capacity)
        grow(m_capacity * 2);

    // first insert
    if (m_size == 0)
    {
        m_begin = 0;
        m_last = 0;
    }
    else
    {
        m_last++;
        if (m_last == m_capacity) m_last = 0;
    }

    V* v = new (m_array + m_last) V(value);
    m_size++;
}

template <typename V>
void
TDeque<V>::pop_front(void)
{
    assert(m_size != 0);
    V& v = m_array[m_begin];
    v.~V();

    // last erase
    if (m_size == 1)
    {
        m_begin = -1;
        m_last = -1;
    }
    else
    {
        m_begin++;
        if (m_begin == m_capacity) m_begin = 0;
    }

    m_size--;
}

template <typename V>
void
TDeque<V>::pop_back(void)
{
    assert(m_size != 0);
    V& v = m_array[m_last];
    v.~V();

    // last erase
    if (m_size == 1)
    {
        m_begin = -1;
        m_last = -1;
    }
    else
    {
        m_last--;
        if (m_last == -1) m_last = m_capacity - 1;
    }

    m_size--;
}

template <typename V>
V&
TDeque<V>::front(void)
{
    assert(m_size > 0);
    return m_array[m_begin];
}

template <typename V>
const V&
TDeque<V>::front(void) const
{
    assert(m_size > 0);
    return m_array[m_begin];
}


template <typename V>
V&
TDeque<V>::back(void)
{
    assert(m_size > 0);
    return m_array[m_last];
}

template <typename V>
const V&
TDeque<V>::back(void) const
{
    assert(m_size > 0);
    return m_array[m_last];
}
