#define PROBLEM "https://judge.yosupo.jp/problem/incremental_scc"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/incremental_scc.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

using mint = modint998;

void solve() {
    INT(N, M);
    vector<mint> X(N); read(X);

    Graph<int, 1> G(N);
    G.read_graph(M, 0, 0);

    auto time = incremental_scc(G);
    vector<vector<int>> IDS(M + 1);
    For(i, M) {
        if (time[i] != infty<int>) IDS[time[i]].emplace_back(i);
    }

    UnionFind uf(N);
    mint ANS = 0;
    For(t, 1, M + 1) {
        for (auto &i: IDS[t]) {
            int a = G.edges[i].frm;
            int b = G.edges[i].to;
            a = uf[a], b = uf[b];
            if (a == b) continue;
            ANS += X[a] * X[b];
            uf.merge(a, b);
            X[uf[a]] = X[a] + X[b];
        }
        print(ANS);
    }
}

signed main() { solve(); }
