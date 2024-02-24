#ifndef AJAY_HASH_VECTOR
#define AJAY_HASH_VECTOR

#include "library/random/base.hpp"
#include "library/mod/modint61.hpp"

namespace mitsuha{
template <typename T>
unsigned long long hash_vector(vector<T> X) {
    using mint = modint61;
    static vector<mint> hash_base;
    int n = len(X);
    while (len(hash_base) <= n) { hash_base.eb(RNG(mint::get_mod())); }
    mint H = 0;
    For(i, n) H += hash_base[i] * mint(X[i]);
    H += hash_base[n];
    return H.val;
}
} // namespace mitsuha
#endif // AJAY_HASH_VECTOR