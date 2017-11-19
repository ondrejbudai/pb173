#include "int16_set_bitvector.hh"
#include "int16_set_trie.hh"

#include <iostream>
#include <cassert>

template<typename Set>
void test(){
    Set set;
    assert(!set.contains(1));
    set.insert(1);
    assert(set.contains(1));

    assert(!set.contains(0));
    set.insert(0);
    assert(set.contains(0));
    assert(set.contains(1));


    assert(!set.contains(793));
    set.insert(793);
    assert(set.contains(793));


    assert(!set.contains(65535));
    set.insert(65535);
    assert(set.contains(65535));

    Set set2;
    set.insert(0);
    set.insert(2);
    set.insert(66);
    set.insert(388);
    set.insert(1);

    Set set3 = set | set2;
    assert(set.contains(0));
    assert(set.contains(1));
    assert(set.contains(2));
    assert(set.contains(66));
    assert(set.contains(388));
    assert(set.contains(793));
    assert(set.contains(65535));
    assert(!set.contains(4));
    assert(!set.contains(46));
    assert(!set.contains(492));
    assert(!set.contains(492));
    assert(!set.contains(65534));
}

int main() {
    test<int16_set_bitvector>();
    test<int16_set_trie>();
    return 0;
}
