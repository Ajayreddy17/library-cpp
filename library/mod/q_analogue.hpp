#ifndef AJAY_Q_ANALOGUE
#define AJAY_Q_ANALOGUE

#include "library/mod/modint.hpp"
#include "library/mod/all_inverse.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/q_analogue.hpp
// q-factorials [n]_q! and q-binomials mod a prime, n <= LIM
// [n]_q = 1 + q + ... + q^{n-1}; D = first n with [n]_q = 0 (q-Lucas beyond it)
template <typename mint>
struct q_Analogue {
    const mint q;
    const int LIM;
    int D;
    vector<mint> factorial, ifactorial;

    q_Analogue(mint q, int LIM) : q(q), LIM(LIM) {
        assert(LIM < mint::get_mod());
        build();
    }

    void build() {
        factorial.reserve(LIM + 1);
        factorial.emplace_back(1);
        mint x = 1;
        For(i, 1, LIM + 1) {
            if (x == mint(0)) break;
            factorial.emplace_back(factorial.back() * x);
            x = q * x + mint(1);
        }
        ifactorial = all_inverse(factorial);
        D = len(factorial);
    }

    mint fact(int N) {
        assert(0 <= N && N <= LIM);
        return (N < D ? factorial[N] : mint(0));
    }
    mint fact_inv(int N) {
        assert(0 <= N && N < D);
        return ifactorial[N];
    }
    mint binom(int N, int K) {
        assert(0 <= N && N <= LIM);
        if (K < 0 || K > N) return mint(0);
        if (N < D) return factorial[N] * ifactorial[K] * ifactorial[N - K];
        auto [n1, n2] = divmod(N, D);
        auto [k1, k2] = divmod(K, D);
        return C<mint>(n1, k1) * binom(n2, k2);
    }
};
} // namespace mitsuha
#endif // AJAY_Q_ANALOGUE
