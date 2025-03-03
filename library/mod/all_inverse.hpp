#ifndef AJAY_ALL_INVERSE
#define AJAY_ALL_INVERSE

#include "library/mod/modint.hpp"

namespace mitsuha{
template <typename mint>
vector<mint> all_inverse(vector<mint>& X) {
    for (auto&& x : X) assert(x != mint(0));
    int N = len(X);
    vector<mint> res(N + 1);
    res[0] = mint(1);
    For(i, N) res[i + 1] = res[i] * X[i];
    mint t = res.back().inverse();
    res.pop_back();
    Frr(i, N) {
        res[i] *= t;
        t *= X[i];
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_ALL_INVERSE