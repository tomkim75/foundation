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

    TDequeItrBase(Deque* deque, size_t pos) : m_deque(deque), m_pos(pos) { }
    TDequeItrBase(TDequeItrBase& other) : m_deque(other.m_deque), m_pos(other.m_pos) { }

    size_t getArrayPos(void) const;
    void increment(void);
    void decrement(void);
    bool equals(const TDequeItrBase& other) const;

    Deque* m_deque;
    size_t m_pos;
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

    TDequeItr(Deque* deque, size_t pos) : TDequeItrBase(deque, pos) { }
    TDequeItrBase<V> asBase(void) const { return TDequeItrBase<V>(m_deque, m_pos); }

    static TDequeItr<V> begin(Deque* deque) { return TDequeItr(deque, deque->m_begin); }
    static TDequeItr<V> last(Deque* deque) { return TDequeItr(deque, (deque->m_begin <= deque->m_last) ? deque->m_last : deque->m_capacity + deque->m_last); }
    static TDequeItr<V> end(Deque* deque) { return TDequeItr(deque, -1); }
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

    TDequeConstItr(Deque* deque, size_t pos) : TDequeItrBase(deque, pos) { }
    TDequeItrBase<V> asBase(void) const { return TDequeItrBase<V>(m_deque, m_pos); }

    static TDequeConstItr<V> begin(Deque* deque) { return TDequeConstItr(deque, deque->m_begin); }
    static TDequeConstItr<V> last(Deque* deque) { return TDequeItr(deque, (deque->m_begin <= deque->m_last) ? deque->m_last : deque->m_capacity + deque->m_last); }
    static TDequeConstItr<V> end(Deque* deque) { return TDequeConstItr(deque, -1); }
};

template<typename V>
bool
TDequeItrBase<V>::valid(void) const
{
    size_t pos = getArrayPos();
    return m_deque->inBounds(m_pos);
}

template<typename V>
size_t
TDequeItrBase<V>::getArrayPos(void) const
{
    if (m_pos == -1)
        return -1;
    else
        return m_pos % m_deque->m_capacity;
}

template<typename V>
void
TDequeItrBase<V>::increment(void)
{
    size_t pos = getArrayPos();

    assert(m_deque->inBounds(pos));
    assert(m_deque->m_size != 0);

    if (pos == m_deque->m_last)
        m_pos = -1;
    else
        m_pos++;
}

template<typename V>
void
TDequeItrBase<V>::decrement(void)
{
    size_t pos = getArrayPos();

    assert(m_deque->m_size != 0);
    assert(m_deque->inBounds(m_pos));

    if (pos == m_deque->m_begin)
    {
        m_pos = -1;
    }
    else
    {        
        m_pos--;

        if (m_pos == -1)
        {
            // pos was 0 and was in bounds, so the next valid pos is one exists
            // should be at right edge
            m_pos = m_deque->m_capacity - 1;
        }
    }
}

template<typename V>
bool
TDequeItrBase<V>::equals(const TDequeItrBase& other) const
{
    assert(m_deque == other.m_deque);
    return getArrayPos() == other.getArrayPos();
}

template <typename V>
V&
TDequeItr<V>::operator*(void)
{
    size_t pos = getArrayPos();
    assert(m_deque->inBounds(pos));
    return m_deque->m_array[pos];
}

template <typename V>
const V&
TDequeConstItr<V>::operator*(void) const
{
    size_t pos = getArrayPos();
    assert(m_deque->inBounds(pos));
    return m_deque->m_array[pos];
}
