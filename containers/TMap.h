/*
Copyright 2016 Tom Kim
Implementation of a map container that stores key-value pairs backed by a
red-black tree with an STL-like interface.

Example:

    typedef TMap<std::string, std::string> Car;
    Car car;
    car.insert("model", "volkswagen");
    car.insert("make", "jetta");
    car.insert("year", "1995");
    car.insert("color", "black");

    Car::iterator itr = car.find("color");
    itr->second = "red";                    // change "color: black" to "color: red"
    itr = car.find("year");
    car.erase(itr);                         // erase "year: 1995"

    for (Car::const_iterator itr = car.begin(); itr != car.end(); ++itr)
    {
        std::cout << itr->first << ": " << itr->second << std::endl;
    }
*/

#pragma once

#include "TMap.h"

template <typename K, typename V>
class TMapPair
{
    template <typename K, typename V> friend class TMap;
    template <typename K, typename V> friend class TMapItr;
    template <typename K, typename V> friend class TMapConstItr;

public:

    TMapPair(const K& key) : first(key) { }
    TMapPair(const K& key, const V& value) : first(key), second(value) { }
    TMapPair(const TMapPair& other) : first(other.first), second(other.second) { }
    TMapPair(void) { }

    bool operator<(const TMapPair& other) const { return first < other.first; }
    TMapPair* operator->(void) { return this; }
    const TMapPair* operator->(void) const { return this; }

    K first;
    V second;
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
    V& operator*(void) { Pair& pair = *m_baseItr; return pair.second; }
    Pair& operator->(void) { Pair& pair = *m_baseItr; return pair; }

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
    const V& operator*(void) const { const Pair& pair = *m_baseItr; return pair.second; }
    const Pair& operator->(void) const { const Pair& pair = *m_baseItr; return pair; }

private:

    BaseItr m_baseItr;

    TMapConstItr(const BaseItr& baseItr) : m_baseItr(baseItr) { }
};

template <typename K, typename V>
class TMap : private TRbTree<TMapPair<K, V> >
{
    typedef TMapPair<K, V> Pair;

public:

    typedef TMapItr<K, V> iterator;
    typedef TMapConstItr<K, V> const_iterator;

    void insert(const K& key, const V& value) { TRbTree::insert(Pair(key, value)); }
    void erase(const K& key) { TRbTree::erase(Pair(key)); }
    void erase(iterator itr) { TRbTree::erase(itr.m_baseItr); }

    iterator find(const K& key) { return iterator(TRbTree::find(Pair(key))); }
    iterator begin(void) { return iterator(TRbTree::begin()); }
    iterator end(void) { return iterator(TRbTree::end()); }
    iterator last(void) { return iterator(TRbTree::last()); }

    const_iterator find(const K& key) const { return const_iterator(TRbTree::find(Pair(key))); }
    const_iterator begin(void) const { return const_iterator(TRbTree::begin()); }
    const_iterator end(void) const { return const_iterator(TRbTree::end()); }
    const_iterator last(void) const { return const_iterator(TRbTree::last()); }
};
