/*
Copyright 2016 Tom Kim
Implementation of a set container backed by a red-black tree with an STL-like
interface.

Example:

    typedef TSet<std::string> Car;
    Car car;
    car.insert("volkswagen");
    car.insert("jetta");
    car.insert("1995");
    car.insert("black");

    Car::iterator itr = car.find("black");
    car.erase(itr);                         // erase "black"
    car.insert("red");                      // insert "red"

    for (Car::const_iterator itr = car.begin(); itr != car.end(); ++itr)
    {
        std::cout << *itr << std::endl;
    }
*/
#pragma once

#include "TSet.h"

template <typename K>
class TSet : private TRbTree<K>
{
public:

    typedef TRbTreeItr<K> iterator;
    typedef TRbTreeConstItr<K> const_iterator;

    void insert(const K& key) { TRbTree::insert(key); }
    void erase(const K& key) { TRbTree::erase(key); }
    void erase(const_iterator itr) { TRbTree::erase(itr); }

    iterator find(const K& key) { return iterator(TRbTree::find(key)); }
    iterator begin(void) { return iterator(TRbTree::begin()); }
    iterator end(void) { return iterator(TRbTree::end()); }
    iterator last(void) { return iterator(TRbTree::last()); }

    const_iterator find(const K& key) const { return const_iterator(TRbTree::find(Pair(key))); }
    const_iterator begin(void) const { return const_iterator(TRbTree::begin()); }
    const_iterator end(void) const { return const_iterator(TRbTree::end()); }
    const_iterator last(void) const { return const_iterator(TRbTree::last()); }
};
