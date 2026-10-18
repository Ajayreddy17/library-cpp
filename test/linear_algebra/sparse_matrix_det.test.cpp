#define PROBLEM "https://judge.yosupo.jp/problem/sparse_matrix_det"
#include <library/template.hpp>
#include <library/linear_algebra/blackbox/det.hpp>

#include <library/mod/modint.hpp>
using mint = modint998;

void solve() {
    LL(N, K);
    using T = tuple<int, int, mint>;
    VT(T, dat, K);
    auto A = [&](vector<mint> X) -> vector<mint> {
        vector<mint> Y(N);
        for (auto& [a, b, c]: dat) Y[b] += X[a] * c;
        return Y;
    };
    mint d = blackbox_det<mint>(N, A);
    print(d);
}

signed main() {
    solve();
    return 0;
}
