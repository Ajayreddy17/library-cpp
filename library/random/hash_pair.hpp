#ifndef AJAY_HASH_PAIR
#define AJAY_HASH_PAIR

#include "library/random/base.hpp"

namespace mitsuha{
template <typename T>
uint64_t hash_pair(pair<T, T> X) {
    static long long hash_base = 0;
    if (hash_base == 0) hash_base = RNG_64();
    return hash_base * X.first + X.second;
}
} // namespace mitsuha
#endif // AJAY_HASH_PAIR
