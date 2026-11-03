#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/coloring/bipartite_edge_coloring.hpp"

// edges are given in random orientation (left->right or right->left) and with arbitrary costs
void test() {
    For(iter, 500) {
        int L = RNG(1, 8), R = RNG(1, 8), M = RNG(0, 30);
        int N = L + R;
        Graph<int, 0> G(N);
        vector<int> deg(N);
        For(M) {
            int a = RNG(0, L), b = L + RNG(0, R);
            if (RNG(0, 2)) swap(a, b);
            G.add(a, b, RNG(0, 10));
            deg[a]++, deg[b]++;
        }
        G.build();
        int D = *max_element(deg.begin(), deg.end());
        auto [C, color] = bipartite_edge_coloring(G);
        assert(C == D && len(color) == M);
        vector<set<int>> used(N);
        For(i, M) {
            auto &e = G.edges[i];
            assert(0 <= color[i] && color[i] < C);
            assert(!used[e.frm].count(color[i]) && !used[e.to].count(color[i]));
            used[e.frm].insert(color[i]), used[e.to].insert(color[i]);
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test();
    solve();
    return 0;
}
