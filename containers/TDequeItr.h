/*
Copyright 2016 Tom Kim
Implementation of a deque iterator with an STL-like interface.
*/
#pragma once

#include "TDeque.h"

template <typename V> class TDeque;

template <typename V>
class TDequeItrBase
{
    typedef TDeque<V> Deque;
    template <typename V> friend class TDeque;
    template <typename V> friend class TDequeItr;
    template <typename V> friend class TDequeConstItr;

protected:

    TDequeItrBase(Deque* deque, size_t pos, size_t from) : m_deque(deque), m_pos(pos), m_from(from) { }
    TDequeItrBase(TDequeItrBase& other) : m_deque(other.m_deque), m_pos(other.m_pos), m_from(other.m_from) { }

    void increment(void);
    void decrement(void);
    bool equals(const TDequeItrBase& other) const;

    Deque* m_deque;
    size_t m_pos;
    size_t m_from;
};

template <typename V>
class TDequeItr : private TDequeItrBase<V>
{
    typedef TDeque<V> Deque;
    template <typename V> friend class TDeque;
    template <typename V> friend class TDequeConstItr;

public:

    TDequeItr(const TDequeItr& itr) : TDequeItrBase(itr.asBase()) { }

    bool operator==(const TDequeItr& other) const { return equals(asBase()); }
    bool operator!=(const TDequeItr& other) const { return !equals(asBase()); }
    bool operator==(const TDequeConstItr<V>& other) const { return equals(asBase()); }
    bool operator!=(const TDequeConstItr<V>& other) const { return !equals(asBase()); }
    TDequeItr& operator++(void) { increment(); return *this; }
    TDequeItr& operator--(void) { decrement(); return *this; }
    V& operator*(void);

private:

    TDequeItr(Deque* deque, size_t pos, size_t from) : TDequeItrBase(deque, pos, from) { }
    TDequeItrBase<V> asBase(void) const { return TDequeItrBase<V>(m_deque, m_pos, m_from); }
};

template <typename V>
class TDequeConstItr : private TDequeItrBase<V>
{
    typedef TDeque<V> Deque;
    template <typename V> friend class TDeque;
    template <typename V> friend class TDequeItr;

public:

    TDequeConstItr(const TDequeConstItr& itr) : TDequeItrBase(itr.asBase()) { }
    TDequeConstItr(const TDequeItr<V>& itr) : TDequeItrBase(itr.asBase()) { }

    bool operator==(const TDequeConstItr& other) const { return equals(asBase()); }
    bool operator!=(const TDequeConstItr& other) const { return !equals(asBase()); }
    bool operator==(const TDequeItr<V>& other) const { return equals(asBase()); }
    bool operator!=(const TDequeItr<V>& other) const { return !equals(asBase()); }
    TDequeConstItr& operator++(void) { increment(); return *this; }
    TDequeConstItr& operator--(void) { decrement(); return *this; }
    const V& operator*(void) const;

private:

    TDequeConstItr(Deque* deque, size_t pos, size_t from) : TDequeItrBase(deque, pos, from) { }
    TDequeItrBase<V> asBase(void) const { return TDequeItrBase<V>(m_deque, m_pos, m_from); }
};

template<typename V>
void
TDequeItrBase<V>::increment(void)
{
    assert(m_deque->m_size != 0);

    if (m_from < m_deque->m_capacity)
    {
        m_pos = m_pos + m_from;
        m_from = 0;
    }

    //assert(m_deque->inBounds(m_pos));

    if (m_pos == m_deque->m_last || !m_deque->inBounds(m_pos))
    {
        m_pos = -1;
        m_from = 0;
    }
    else
    {
        m_pos++;

        if (m_pos == m_deque->m_capacity)
        {
            m_pos = 0;
            m_from = m_deque->m_capacity;
        }
    }
}

template<typename V>
void
TDequeItrBase<V>::decrement(void)
{
    assert(m_deque->m_size != 0);

    if (m_from < m_deque->m_capacity)
    {
        m_pos = m_pos + m_from;
        m_from = 0;
    }

    //assert(m_deque->inBounds(m_pos));

    if (m_pos == m_deque->m_begin || !m_deque->inBounds(m_pos))
    {
        m_pos = -1;
        m_from = 0;
    }
    else
    {
        m_pos--;

        if (m_pos == -1)
        {
            m_pos = m_deque->m_capacity - 1;
            m_from = 0;
        }
    }
}

template<typename V>
bool
TDequeItrBase<V>::equals(const TDequeItrBase& other) const
{
    return ((m_pos == -1 && other.m_pos == -1) || ((m_pos + m_from) == (other.m_pos + other.m_from)));
}

template <typename V>
V&
TDequeItr<V>::operator*(void)
{
    assert(m_pos != -1);

    if (m_from < m_deque->m_capacity)
    {
        m_pos = m_pos + m_from;
        m_from = 0;
    }

    //assert(m_deque->inBounds(m_pos));
    return m_deque->m_array[m_pos];
}

template <typename V>
const V&
TDequeConstItr<V>::operator*(void) const
{
    assert(m_pos != -1);

    if (m_from < m_deque->m_capacity)
    {
        m_pos = m_pos + m_from;
        m_from = 0;
    }

    //assert(m_deque->inBounds(m_pos));
    return m_deque->m_array[m_pos];
}
