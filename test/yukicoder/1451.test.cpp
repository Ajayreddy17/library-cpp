#define PROBLEM "https://yukicoder.me/problems/no/1451"
#include "library/template.hpp"
#include "library/graph/tree_of_unionfind.hpp"
#include "library/graph/tree.hpp"
#include "library/mod/modint.hpp"

using mint = modint107;

void solve() {
    LL(N, M);
    using P = pair<int, int>;
    vector<P> AB(M); read(AB);
    for (auto&& [a, b]: AB) --a, --b;
    auto G = tree_of_unionfind(N, AB, true);
    Tree<decltype(G)> tree(G, N + M);

    vector<mint> ANS(N);
    vector<long long> SZ(N + M + 1);
    For(v, N) SZ[v] = 1;
    auto V = tree.V;
    Frr(i, len(V)) {
        int v = V[i];
        int p = tree.parent[v];
        if (p != -1) SZ[p] += SZ[v];
    }

    auto dfs = [&](auto& dfs, int v, mint p) -> void {
        if (v < N) {
            ANS[v] = p;
            return;
        }

        auto ch = tree.collect_child(v);
        if (len(ch) == 1) return dfs(dfs, ch[0], p);
        int a = ch[0], b = ch[1];
        if (SZ[a] == SZ[b]) {
            dfs(dfs, a, p * inv<mint>(2));
            dfs(dfs, b, p * inv<mint>(2));
        }
        if (SZ[a] > SZ[b]) { dfs(dfs, a, p); }
        if (SZ[a] < SZ[b]) { dfs(dfs, b, p); }
    };

    for (auto&& e: G[N + M]) dfs(dfs, e.to, mint(1));
    for (auto&& x: ANS) print(x);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
