#ifndef AJAY_TO_SMALL_KEY
#define AJAY_TO_SMALL_KEY

#include "library/datastructure/hashmap.hpp"

namespace mitsuha{
// [30,10,20,30] -> [0,1,2,0] etc.
struct To_Small_Key {
    int kind = 0;
    HashMap<int> MP;

    To_Small_Key(unsigned int n = 0) : MP(n) {}

    void reserve(unsigned int n) { MP.build(n); }

    int set_key(unsigned long long x) {
        if (!MP.count(x)) MP[x] = kind++;
        return MP[x];
    }

    int query(unsigned long long x) { return MP.get(x, -1); }
};
} // namespace mitsuha
#endif // AJAY_TO_SMALL_KEY