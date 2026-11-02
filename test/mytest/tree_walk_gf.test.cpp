#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/tree_walk_generating_function.hpp"
#include "library/poly/fps_div.hpp"

using mint = modint998;

template <bool ALLOW_LOOP>
void test() {
    For(N, 1, 30) {
        For(50) {
            Graph<int, 0> G(N);
            vector<vector<mint>> mat(N, vector<mint>(N));
            for (auto& [a, b]: random_tree(N)) {
                G.add(a, b);
                mat[a][b] = RNG(0, mint::get_mod());
                mat[b][a] = RNG(0, mint::get_mod());
            }
            G.build();
            if (ALLOW_LOOP) { For(v, N) mat[v][v] = RNG(mint::get_mod()); }
            int s = RNG(0, N), t = RNG(0, N);
            int LIM = 200;
            vector<mint> ANS(LIM + 1);
            vector<mint> dp(N);
            dp[s] = 1;
            For(k, LIM) {
                ANS[k] = dp[t];
                vector<mint> newdp(N);
                For(a, N) For(b, N) newdp[b] += dp[a] * mat[a][b];
                swap(dp, newdp);
                ANS[k + 1] = dp[t];
            }
            auto [f, g] = tree_walk_generating_function<ALLOW_LOOP, mint>(
                    G, s, t, [&](int i, int j) -> mint { return mat[i][j]; });
            f.resize(LIM + 1);
            g.resize(LIM + 1);
            f = fps_div(f, g);
            assert(f == ANS);
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
}

signed main() {
    test<0>();
    test<1>();
    solve();
    return 0;
}
