#ifndef AJAY_TO_SMALL_KEY
#define AJAY_TO_SMALL_KEY

#include "library/datastructure/hashmap.hpp"

namespace mitsuah{
// [30,10,20,30] -> [0,1,2,0] etc.
template <int LOG = 20, bool USE_RESET = false>
struct To_Small_Key {
    int kind = 0;
    HashMap<int, LOG, true> MP;

    int set_key(unsigned long long x) {
        int idx = MP.index(x);
        if (!MP.used[idx]) {
            MP.used[idx] = 1;
            MP.key[idx] = x;
            MP.val[idx] = kind++;
        }
        return MP.val[idx];
    }

    int query(unsigned long long x) { return MP.get(x, -1); }

    void reset() {
        static_assert(USE_RESET);
        MP.reset();
    }
};
} // namespace mitsuha
#endif // AJAY_TO_SMALL_KEY