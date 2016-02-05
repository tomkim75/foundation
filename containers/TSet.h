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

    iterator find(const K& key) { return Iterator(TRbTree::find(key)); }
    iterator begin() { return iterator(TRbTree::begin()); }
    iterator end() { return iterator(TRbTree::end()); }
    iterator last() { return iterator(TRbTree::last()); }
};
