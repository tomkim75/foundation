#pragma once

#include "TSet.h"

template <typename K>
class TSet : private TRbTree<K>
{
public:

    typedef TRbTreeItr<K> Iterator;
    typedef TRbTreeConstItr<K> ConstIterator;

    void insert(const K& key) { TRbTree::insert(key); }
    void erase(const K& key) { TRbTree::erase(key); }
    void erase(Iterator itr) { TRbTree::erase(itr); }

    Iterator find(const K& key) { return Iterator(TRbTree::find(key)); }
    Iterator begin() { return Iterator(TRbTree::begin()); }
    Iterator end() { return Iterator(TRbTree::end()); }
    Iterator last() { return Iterator(TRbTree::last()); }
};
