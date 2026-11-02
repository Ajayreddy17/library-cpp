#define PROBLEM "https://yukicoder.me/problems/no/1796"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/centroid_decomposition_old.hpp"
#include "library/poly/convolution.hpp"

using mint = modint998;

void solve() {
    LL(N);
    vector<mint> A(N); read(A);
    Graph<int, 0> G(N);
    G.read_tree();
    vector<mint> ANS(N);

    Centroid_Decomposition<decltype(G)> X(G);
    For(v, N) {
        auto [V, dp, indptr] = X.collect_dist(v);
        auto calc = [&](vector<int> V, vector<int> dp, int sgn) -> void {
            vector<mint> F;
            int n = len(V);
            For(i, n) {
                int v = V[i], dv = dp[i];
                if (dv >= len(F)) F.resize(dv + 1);
                F[dv] += A[v];
            }
            int s = len(F);
            reverse(F.begin(), F.end());
            vector<mint> F1(s + s);
            For(i, len(F1)) F1[i] = inv<mint>(i + 1) * inv<mint>(i + 1);
            F = convolution(F, F1);
            F = {F.begin() + s - 1, F.end()};
            if (sgn == -1) {
                for (auto&& x: F) x = -x;
            }
            For(i, n) {
                int v = V[i], dv = dp[i];
                ANS[v] += F[dv];
            }
        };
        calc(V, dp, 1);
        For(i, 1, len(indptr) - 1) {
            int l = indptr[i], r = indptr[i + 1];
            calc({V.begin() + l, V.begin() + r}, {dp.begin() + l, dp.begin() + r},
                      -1);
        }
    }

    mint k0 = fact<mint>(N);
    k0 *= k0;
    for (auto&& x: ANS) x *= k0;
    for (auto&& x: ANS) print(x);
}

signed main() {
    solve();
    return 0;
}
