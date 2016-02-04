#pragma once

#include "TMap.h"

template <typename K, typename V>
class TMapPair
{
    template <typename K, typename V> friend class TMap;
    template <typename K, typename V> friend class TMapItr;
    template <typename K, typename V> friend class TMapConstItr;

public:

    TMapPair(const K& key) : m_key(key) { }
    TMapPair(const K& key, const V& value) : m_key(key), m_value(value) { }
    TMapPair(const TMapPair& other) : m_key(other.m_key), m_value(other.m_value) { }
    TMapPair(void) { }

    bool operator<(const TMapPair& other) const { return m_key < other.m_key; }

private:

    K m_key;
    V m_value;
};

template <typename K, typename V>
class TMapItr
{
    typedef TMapPair<K, V> Pair;
    typedef TRbTreeItr<Pair> BaseItr;
    template <typename K, typename V> friend class TMap;
    template <typename K, typename V> friend class TMapConstItr;

public:

    TMapItr(const TMapItr& other) : m_baseItr(other.m_baseItr) { }

    bool operator==(const TMapItr& other) const { return m_baseItr.operator==(other.m_baseItr); }
    bool operator!=(const TMapItr& other) const { return m_baseItr.operator!=(other.m_baseItr); }
    bool operator==(const TMapConstItr<K, V>& other) const { return m_baseItr.operator==(other.m_baseItr); }
    bool operator!=(const TMapConstItr<K, V>& other) const { return m_baseItr.operator!=(other.m_baseItr); }
    TMapItr& operator++(void) { ++m_baseItr; return *this; }
    TMapItr& operator--(void) { --m_baseItr; return *this; }
    V& operator*(void) { Pair& pair = *m_baseItr; return pair.m_value; }

private:

    BaseItr m_baseItr;

    TMapItr(const BaseItr& baseItr) : m_baseItr(baseItr) { }
};

template <typename K, typename V>
class TMapConstItr
{
    typedef TMapPair<K, V> Pair;
    typedef TRbTreeConstItr<Pair> BaseItr;
    template <typename K, typename V> friend class TMap;
    template <typename K, typename V> friend class TMapItr;

public:

    TMapConstItr(const TMapConstItr& other) : m_baseItr(other.m_baseItr) { }
    TMapConstItr(const TMapItr<K, V>& other) : m_baseItr(other.m_baseItr) { }

    bool operator==(const TMapConstItr& other) const { return m_baseItr.operator==(other.m_baseItr); }
    bool operator!=(const TMapConstItr& other) const { return m_baseItr.operator!=(other.m_baseItr); }
    bool operator==(const TMapItr<K, V>& other) const { return m_baseItr.operator==(other.m_baseItr); }
    bool operator!=(const TMapItr<K, V>& other) const { return m_baseItr.operator!=(other.m_baseItr); }
    TMapConstItr& operator++(void) { ++m_baseItr; return *this; }
    TMapConstItr& operator--(void) { --m_baseItr; return *this; }
    const V& operator*(void) const { const Pair& pair = *m_baseItr; return pair.m_value; }

private:

    BaseItr m_baseItr;

    TMapConstItr(const BaseItr& baseItr) : m_baseItr(baseItr) { }
};

template <typename K, typename V>
class TMap : private TRbTree<TMapPair<K, V> >
{
    typedef TMapPair<K, V> Pair;

public:

    typedef TMapItr<K, V> Iterator;
    typedef TMapConstItr<K, V> ConstIterator;

    void insert(const K& key, const V& value) { TRbTree::insert(Pair(key, value)); }
    void erase(const K& key) { TRbTree::erase(Pair(key)); }
    void erase(Iterator itr) { TRbTree::erase(itr.m_baseItr); }

    Iterator find(const K& key) { return Iterator(TRbTree::find(Pair(key))); }
    Iterator begin() { return Iterator(TRbTree::begin()); }
    Iterator end() { return Iterator(TRbTree::end()); }
    Iterator last() { return Iterator(TRbTree::last()); }
};
