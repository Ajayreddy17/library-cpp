#ifndef AJAY_COUNT_UNLABELED_TREE
#define AJAY_COUNT_UNLABELED_TREE

#include "library/poly/online/online_exp.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/convolution.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_unlabeled_tree.hpp

// https://oeis.org/A000055
// https://oeis.org/A000081
template <typename mint>
vector<mint> count_unlabeled_tree(int N, bool rooted) {
    // f: rooted tree
    // f(x) = x prod (1+x^n+x^{2n}+...)^f_n
    // g(x) = f(x)/x
    // log g(x) = sum f[n]log(1+x^n+x^{2n}+...)
    // f[n] given -> [x^n]g(x) given -> f[n+1] given
    vector<mint> f(N + 1);
    vector<mint> log_g(N);
    Online_Exp<mint> X;
    For(n, 1, N + 1) {
        f[n] = X.query(n - 1, log_g[n - 1]);
        // -log(1-x)=x+xx/2+...
        For(k, 1, (N - 1) / n + 1) { log_g[n * k] += f[n] * inv<mint>(k); }
    }
    if (rooted) return f;
    // here f is unlabeled rooted trees (OEIS A000081)
    // generating function of unlabeled trees: f(x) - 1/2(f(x)^2-f(x^2))
    vector<mint> ff = convolution<mint>(f, f);
    vector<mint> ANS = f;
    For(i, N + 1) ANS[i] -= inv<mint>(2) * ff[i];
    For(i, N + 1) {
        if (2 * i <= N) ANS[2 * i] += inv<mint>(2) * f[i];
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_COUNT_UNLABELED_TREE
