#define PROBLEM "https://yukicoder.me/problems/no/1796"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree/centroid_decomposition.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/middle_product.hpp"

using mint = modint998;

void solve() {
    LL(N);
    VT(mint, A, N);
    Graph<int, 0> G(N);
    G.read_tree();

    vector<mint> ANS(N);
    // distance 0
    For(v, N) ANS[v] += A[v];

    auto F = [&](vector<int>& par, vector<int> vs, int L1, int R1, int L2, int R2) -> void {
        int n = len(par);
        vector<int> dist(n);
        For(i, 1, n) dist[i] = dist[par[i]] + 1;
        auto f = [&](int L1, int R1, int L2, int R2) -> void {
            int n1 = *max_element(dist.begin() + L1, dist.begin() + R1);
            int n2 = *max_element(dist.begin() + L2, dist.begin() + R2);
            vector<mint> a(n1 + n2 + 1), b(n2 + 1);
            For(i, n1 + n2 + 1) a[i] = inv<mint>(1 + i) * inv<mint>(1 + i);
            For(i, L2, R2) { b[dist[i]] += A[vs[i]]; }
            vector<mint> c = middle_product(a, b);
            assert(len(c) == n1 + 1);
            For(i, L1, R1) ANS[vs[i]] += c[dist[i]];
        };
        f(L1, R1, L2, R2);
        f(L2, R2, L1, R1);
    };
    centroid_decomposition<1>(G, F);

    mint k0 = fact<mint>(N);
    k0 *= k0;
    for (auto&& x: ANS) x *= k0;
    for (auto&& x: ANS) print(x);
}

signed main() {
    solve();
    return 0;
}
