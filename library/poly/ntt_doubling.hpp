#ifndef AJAY_NTT_DOUBLING
#define AJAY_NTT_DOUBLING

#include "library/poly/ntt.hpp"

namespace mitsuha{
// Given the length 2^k of the 2^k degree polynomial, set it to 2^k+1
template <typename mint, bool transposed = false>
void ntt_doubling(vector<mint>& a) {
    static array<mint, 30> root;
    static bool prepared = 0;
    if (!prepared) {
        prepared = 1;
        const int rank2 = mint::ntt_info().first;
        root[rank2] = mint::ntt_info().second;
        Frr(i, rank2) { root[i] = root[i + 1] * root[i + 1]; }
    }

    if constexpr (!transposed) {
        const int M = (int)a.size();
        auto b = a;
        ntt(b, 1);
        mint r = 1, zeta = root[((2 * M) == 0 ? -1 : 31 - __builtin_clz((2 * M)))];
        For(i, M) b[i] *= r, r *= zeta;
        ntt(b, 0);
        copy(begin(b), end(b), back_inserter(a));
    } else {
        const int M = len(a) / 2;
        vector<mint> tmp = {a.begin(), a.begin() + M};
        a = {a.begin() + M, a.end()};
        transposed_ntt(a, 0);
        mint r = 1, zeta = root[((2 * M) == 0 ? -1 : 31 - __builtin_clz((2 * M)))];
        For(i, M) a[i] *= r, r *= zeta;
        transposed_ntt(a, 1);
        For(i, M) a[i] += tmp[i];
    }
}
} // namespace mitsuha
#endif // AJAY_NTT_DOUBLING