/*
Copyright 2016 Tom Kim
Implementation of a vector container with an STL-like interface.
*/
#pragma once

template <typename V> class TVector;

template <typename V>
class TVectorItr
{
    typedef TVector<V> Vector;
    friend class Vector;

public:

    TVectorItr(const TVectorItr& itr) : m_vector(itr.m_vector), m_pos(itr.m_pos) { }

    bool operator==(const TVectorItr& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TVectorItr& other) const { return m_pos != other.m_pos; }
    //bool operator==(const TVectorConstItr<V>& other) const { return m_pos == other.m_pos; }
    //bool operator!=(const TVectorConstItr<V>& other) const { return m_pos != other.m_pos; }
    TListItr& operator++(void) { assert(m_size != -1); m_pos++; if (m_pos == m_vector.m_size) m_pos = -1; }
    TListItr& operator--(void) { assert(m_size != -1); m_pos--; }
    V& operator*(void) { return m_vector.m_array[m_pos]; }

private:

    Vector* m_vector;
    size_t m_pos;

    TVectorItr(Vector* vector, size_t pos) : m_vector(vector), m_pos(pos) { }
};

template <typename V>
class TVector
{
    typedef TVector<V> Node;

public:

    typedef TVectorItr<V> iterator;
    //typedef TVectorConstItr<V> const_iterator;

    TVector(void);
    TVector(size_t capacity);

    void push_back(const V& value);
    void pop_back(void);

    V& back(void);
    const V& back(void) const;

    iterator begin(void) { return iterator(0); }
    iterator last(void) { return iterator(m_size - 1); }
    iterator end(void) { return iterator(-1); }

private:

    V* m_array;
    size_t m_capacity;
    size_t m_size;
};

template <typename V>
TVector<V>::TVector(void)
    : m_array(NULL), m_capacity(0), m_size(0)
{ }

template <typename V>
TVector<V>::TVector(size_t capacity)
    : m_array(NULL), m_capacity(capacity), m_size(0)
{
    m_array = static_cast<V*>(operator new(sizeof(V) * m_capacity));
}

template <typename V>
void
TVector<V>::push_back(const V& value)
{
    assert(m_size + 1 <= m_capacity);
    V* v = new (m_array + m_size) V(value);
    m_size++;
}

template <typename V>
void
TVector<V>::pop_back(void)
{
    assert(m_size != 0);
    V* v = &(m_array[m_size - 1]);
    v->~V();
    m_size--;
}

template <typename V>
V&
TVector<V>::back(void)
{
    assert(m_size + 1 <= m_capacity);
    return m_array[m_size - 1];
}

template <typename V>
const V&
TVector<V>::back(void) const
{
    assert(m_size + 1 <= m_capacity);
    return m_array[m_size - 1];
}
