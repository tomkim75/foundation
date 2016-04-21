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

public:

    bool valid(void) const;

protected:

    TDequeItrBase(Deque* deque, size_t pos, size_t from) : m_deque(deque), m_pos(pos), m_from(from) { }
    TDequeItrBase(TDequeItrBase& other) : m_deque(other.m_deque), m_pos(other.m_pos), m_from(other.m_from) { }

    void normalize(void) const;
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

    bool operator==(const TDequeItr& other) const { return equals(other.asBase()); }
    bool operator!=(const TDequeItr& other) const { return !equals(other.asBase()); }
    bool operator==(const TDequeConstItr<V>& other) const { return equals(other.asBase()); }
    bool operator!=(const TDequeConstItr<V>& other) const { return !equals(other.asBase()); }
    TDequeItr& operator++(void) { increment(); return *this; }
    TDequeItr& operator--(void) { decrement(); return *this; }
    V& operator*(void);

    bool valid(void) const { return TDequeItrBase::valid(); }

private:

    TDequeItr(Deque* deque, size_t pos, size_t from) : TDequeItrBase(deque, pos, from) { }
    TDequeItrBase<V> asBase(void) const { return TDequeItrBase<V>(m_deque, m_pos, m_from); }

    static TDequeItr<V> begin(Deque* deque) { return TDequeItr(deque, deque->m_begin, 0); }
    static TDequeItr<V> last(Deque* deque) { return TDequeItr(deque, deque->m_last, (deque->m_begin <= deque->m_last) ? 0 : deque->m_capacity); }
    static TDequeItr<V> end(Deque* deque) { return TDequeItr(deque, -1, 0); }
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

    bool operator==(const TDequeConstItr& other) const { return equals(other.asBase()); }
    bool operator!=(const TDequeConstItr& other) const { return !equals(other.asBase()); }
    bool operator==(const TDequeItr<V>& other) const { return equals(other.asBase()); }
    bool operator!=(const TDequeItr<V>& other) const { return !equals(other.asBase()); }
    TDequeConstItr& operator++(void) { increment(); return *this; }
    TDequeConstItr& operator--(void) { decrement(); return *this; }
    const V& operator*(void) const;

    bool valid(void) const { return TDequeItrBase::valid(); }

private:

    TDequeConstItr(Deque* deque, size_t pos, size_t from) : TDequeItrBase(deque, pos, from) { }
    TDequeItrBase<V> asBase(void) const { return TDequeItrBase<V>(m_deque, m_pos, m_from); }

    static TDequeConstItr<V> begin(Deque* deque) { return TDequeConstItr(deque, deque->m_begin, 0); }
    static TDequeConstItr<V> last(Deque* deque) { return TDequeConstItr(deque, deque->m_last, (deque->m_begin <= deque->m_last) ? 0 : deque->m_capacity); }
    static TDequeConstItr<V> end(Deque* deque) { return TDequeConstItr(deque, -1, 0); }
};

template<typename V>
bool
TDequeItrBase<V>::valid(void) const
{
    normalize();
    return m_deque->inBounds(m_pos);
}

template<typename V>
void
TDequeItrBase<V>::normalize(void) const
{
    TDequeItrBase* me = const_cast<TDequeItrBase*>(this);

    if (m_pos == -1)
    {
        me->m_from = 0;
    }
    else if (m_from < m_deque->m_capacity)
    {
        me->m_pos = m_pos + m_from;
        me->m_from = 0;
    }
}

template<typename V>
void
TDequeItrBase<V>::increment(void)
{
    normalize();

    assert(m_deque->m_size != 0);
    assert(m_deque->inBounds(m_pos));

    if (m_pos == m_deque->m_last)
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
    normalize();

    assert(m_deque->m_size != 0);
    assert(m_deque->inBounds(m_pos));

    if (m_pos == m_deque->m_begin)
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
    normalize();
    other.normalize();
    assert(m_deque == other.m_deque);
    return m_pos == other.m_pos;
}

template <typename V>
V&
TDequeItr<V>::operator*(void)
{
    normalize();
    assert(m_deque->inBounds(m_pos));
    return m_deque->m_array[m_pos];
}

template <typename V>
const V&
TDequeConstItr<V>::operator*(void) const
{
    normalize();
    assert(m_deque->inBounds(m_pos));
    return m_deque->m_array[m_pos];
}
