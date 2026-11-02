#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/datastructure/bit_array.hpp"
#include "library/flow/bipartite.hpp"
#include "library/flow/bipartite_matching_dense.hpp"
#include "library/flow/rank_maximal_bipartite_matching.hpp"

// dense matching == graph matching; result is a valid matching; cover is a vertex cover of that size
template <typename BS>
void test_dense() {
    For(iter, 1000) {
        int n1 = RNG(1, 12), n2 = RNG(1, 12);
        vector<BS> adj(n1);
        if constexpr (is_same_v<BS, Bit_Array>) For(i, n1) adj[i] = Bit_Array(n2);
        Graph<int, 0> G(n1 + n2);
        int p = RNG(0, 101);
        For(i, n1) For(j, n2) if (RNG(0, 100) < p) adj[i][j] = 1, G.add(i, n1 + j);
        G.build();
        Bipartite_Matching_Dense<BS> D(adj, n1, n2);
        BipartiteMatching<decltype(G)> B(G);
        auto M = D.matching();
        assert(len(M) == len(B.matching()));
        vector<int> u1(n1), u2(n2);
        for (auto [a, b]: M) assert(adj[a][b] && !u1[a]++ && !u2[b]++);
        auto [C1, C2] = D.vertex_cover();
        assert(len(C1) + len(C2) == len(M));
        vector<int> c1(n1), c2(n2);
        for (int a: C1) c1[a] = 1;
        for (int b: C2) c2[b] = 1;
        For(i, n1) For(j, n2) if (adj[i][j]) assert(c1[i] || c2[j]);
    }
}

// rank maximal: (# weight K-1, # weight K-2, ...) is lex max; brute over edge subsets
void test_rank_maximal() {
    For(iter, 1000) {
        int n1 = RNG(1, 5), n2 = RNG(1, 5), m = RNG(0, 10), K = RNG(1, 4);
        Graph<int, 0> G(n1 + n2);
        vector<tuple<int, int, int>> es;
        For(m) {
            int a = RNG(0, n1), b = n1 + RNG(0, n2), w = RNG(0, K);
            G.add(a, b, w), es.emplace_back(a, b, w);
        }
        G.build();
        int KK = 0;
        for (auto [a, b, w]: es) chmax(KK, w + 1);
        auto key = [&](vector<int> cnt) { reverse(cnt.begin(), cnt.end()); return cnt; };
        vector<int> best(KK);
        For(mask, 1 << m) {
            vector<int> used(n1 + n2), cnt(KK);
            bool ok = true;
            For(i, m) if (mask >> i & 1) {
                auto [a, b, w] = es[i];
                if (used[a]++ || used[b]++) ok = false;
                cnt[w]++;
            }
            if (ok && key(cnt) > key(best)) best = cnt;
        }
        Rank_Maximal_Bipartite_Matching<decltype(G)> R(G);
        vector<int> cnt(KK);
        // count matched pairs by the best edge weight between them
        For(v, n1) {
            int u = R.match[v];
            if (u == -1) continue;
            assert(R.match[u] == v);
            int w = -1;
            for (auto [a, b, ww]: es) if (a == v && b == u) chmax(w, ww);
            assert(w != -1);
            cnt[w]++;
        }
        assert(cnt == best);
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_dense<bitset<16>>();
    test_dense<Bit_Array>();
    test_rank_maximal();
    solve();
    return 0;
}
