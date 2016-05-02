/*
Copyright 2016 Tom Kim
Implementation of a vector container with an STL-like interface.
*/
#pragma once

#include <cstddef>
#include <cassert>

template <typename V> class TVector;

template <typename V>
class TVectorItr
{
    typedef TVector<V> Vector;
    template <typename V> friend class TVector;
    template <typename K> friend class TVectorConstItr;

public:

    TVectorItr(const TVectorItr& itr) : m_vector(itr.m_vector), m_pos(itr.m_pos) { }

    bool operator==(const TVectorItr& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TVectorItr& other) const { return m_pos != other.m_pos; }
    bool operator==(const TVectorConstItr<V>& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TVectorConstItr<V>& other) const { return m_pos != other.m_pos; }
    TVectorItr& operator++(void) { assert(m_vector->m_size != -1); m_pos++; if (m_pos == m_vector->m_size) m_pos = -1; return *this;  }
    TVectorItr& operator--(void) { assert(m_vector->m_size != -1); m_pos--; return *this; }
    V& operator*(void) { return m_vector->m_array[m_pos]; }

private:

    TVectorItr(Vector* vector, size_t pos) : m_vector(vector), m_pos(pos) { }

    Vector* m_vector;
    size_t m_pos;
};

template <typename V>
class TVectorConstItr
{
    typedef TVector<V> Vector;
    template <typename V> friend class TVector;
    template <typename V> friend class TVectorItr;

public:

    TVectorConstItr(const TVectorConstItr& itr) : m_vector(itr.m_vector), m_pos(itr.m_pos) { }
    TVectorConstItr(const TVectorItr<V>& itr) : m_vector(itr.m_vector), m_pos(itr.m_pos) { }

    bool operator==(const TVectorConstItr& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TVectorConstItr& other) const { return m_pos != other.m_pos; }
    bool operator==(const TVectorItr<V>& other) const { return m_pos == other.m_pos; }
    bool operator!=(const TVectorItr<V>& other) const { return m_pos != other.m_pos; }
    TVectorConstItr& operator++(void) { assert(m_vector->m_size != -1); m_pos++; if (m_pos == m_vector->m_size) m_pos = -1; return *this;  }
    TVectorConstItr& operator--(void) { assert(m_vector->m_size != -1); m_pos--; return *this;  }
    const V& operator*(void) const { return m_vector->m_array[m_pos]; }

private:

    TVectorConstItr(Vector* vector, size_t pos) : m_vector(vector), m_pos(pos) { }

    Vector* m_vector;
    size_t m_pos;
};


template <typename V>
class TVector
{
    typedef TVector<V> Node;
    template <typename K> friend class TVectorItr;
    template <typename K> friend class TVectorConstItr;

public:

    typedef TVectorItr<V> iterator;
    typedef TVectorConstItr<V> const_iterator;

    TVector(void);
    TVector(size_t capacity);

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
TVector<V>::TVector(void)
    : m_array(NULL), m_capacity(0), m_size(0)
{ }

template <typename V>
TVector<V>::TVector(size_t capacity)
    : m_array(NULL), m_capacity(capacity), m_size(0)
{
    grow(capacity);
}

template <typename V>
void
TVector<V>::grow(size_t capacity)
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
TVector<V>::push_back(const V& value)
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
TVector<V>::pop_back(void)
{
    assert(m_size != 0);
    V& v = m_array[m_size - 1];
    v.~V();
    m_size--;
}

template <typename V>
V&
TVector<V>::back(void)
{
    assert(m_size > 0);
    return m_array[m_size - 1];
}

template <typename V>
const V&
TVector<V>::back(void) const
{
    assert(m_size > 0);
    return m_array[m_size - 1];
}
