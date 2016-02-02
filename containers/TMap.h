#pragma once

#include "TMap.h"

template <typename K, typename V>
class TMapPair
{
    template <typename K, typename V> friend class TMap;
    template <typename K, typename V> friend class TMapItr;

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

public:

    TMapItr(const TMapItr& other) : m_baseItr(other.m_baseItr) { }

    bool operator==(const TMapItr& other) const { return m_baseItr.operator==(other.m_baseItr); }
    bool operator!=(const TMapItr& other) const { return m_baseItr.operator!=(other.m_baseItr); }
    TMapItr& operator++(void) { ++m_baseItr; return *this; }
    TMapItr& operator--(void) { --m_baseItr; return *this; }
    V& operator*(void) { Pair& pair = *m_baseItr; return pair.m_value; }

private:

    BaseItr m_baseItr;

    TMapItr(const BaseItr& baseItr) : m_baseItr(baseItr) { }
};

template <typename K, typename V>
class TMap : private TRbTree<TMapPair<K, V> >
{
public:

    typedef TMapItr<K, V> Iterator;
    //typedef TRbTreeConstItr<K, V> ConstIterator;

    void insert(const K& key, const V& value) { TRbTree::insert(TMapPair<K, V>(key, value)); }
    void erase(const K& key) { TRbTree::erase(TMapPair(key)); }
    void erase(Iterator itr) { TRbTree::erase(itr.m_baseItr); }

    Iterator find(const K& key) { return Iterator(TRbTree::find(TMapPair<K, V>(key))); }
    Iterator begin() { return Iterator(TRbTree::begin()); }
    Iterator end() { return Iterator(TRbTree::end()); }
    Iterator last() { return Iterator(TRbTree::last()); }
};
