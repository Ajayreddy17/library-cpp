#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/flow/maxflow.hpp"
#include "library/flow/incremental_maxflow.hpp"
#include "library/flow/gomory_hu_tree.hpp"

using E = tuple<int, int, long long>;

// min over vertex subsets S with s in S, t not in S
long long brute_cut(int n, const vector<E>& es, int s, int t) {
    long long best = linf;
    For(m, 1 << n) {
        if (!(m >> s & 1) || (m >> t & 1)) continue;
        long long c = 0;
        for (auto [a, b, w]: es) if ((m >> a & 1) && !(m >> b & 1)) c += w;
        chmin(best, c);
    }
    return best;
}

template <typename MF>
void check_flow(MF& F, int n, const vector<E>& es, int s, int t) {
    long long f = F.flow();
    assert(f == brute_cut(n, es, s, t));
    // cut side is a min cut
    auto [val, side] = F.cut();
    assert(val == f && side[s] == 0 && side[t] == 1);
    long long c = 0;
    for (auto [a, b, w]: es) if (side[a] == 0 && side[b] == 1) c += w;
    assert(c == f);
    // flow conservation and capacity
    vector<long long> bal(n);
    for (auto [a, b, x]: F.get_flow_edges()) bal[a] -= x, bal[b] += x;
    For(v, n) if (v != s && v != t) assert(bal[v] == 0);
    assert(bal[t] == f);
    // path decomposition
    auto P = F.path_decomposition();
    assert(len(P) == f);
    for (auto& p: P) assert(p.front() == s && p.back() == t);
}

void test_maxflow() {
    For(iter, 2000) {
        int n = RNG(2, 8), m = RNG(0, 15);
        int s = RNG(0, n), t = RNG(0, n);
        if (s == t) continue;
        vector<E> es;
        For(m) es.emplace_back(RNG(0, n), RNG(0, n), RNG(0, 6));
        MaxFlow<long long> F(n, s, t);
        for (auto [a, b, w]: es) F.add(a, b, w);
        check_flow(F, n, es, s, t);
        // residual graph: v reachable from s iff side[v] == 0
        auto R = F.res_graph();
        vector<int> vis(n);
        vector<int> st = {s};
        vis[s] = 1;
        while (!st.empty()) {
            int v = st.back(); st.pop_back();
            for (auto& e: R[v]) if (!vis[e.to]) vis[e.to] = 1, st.push_back(e.to);
        }
        auto side = F.cut().second;
        For(v, n) assert(vis[v] == (side[v] == 0));
        // change_capacity, both up and down, compared with a fresh build
        For(q, 5) {
            if (m == 0) break;
            int i = RNG(0, m);
            long long c = RNG(0, 6);
            get<2>(es[i]) = c;
            F.change_capacity(i, c);
            check_flow(F, n, es, s, t);
        }
    }
}

void test_change_before_flow() {
    MaxFlow<int> F(3, 0, 2);
    F.add(0, 1, 5), F.add(1, 2, 5);
    F.change_capacity(0, 2);
    assert(F.flow() == 2);
}

void test_incremental() {
    For(iter, 1000) {
        int n = RNG(2, 8), s = RNG(0, n), t = RNG(0, n);
        if (s == t) continue;
        Incremental_MaxFlow<long long> F(n, s, t);
        vector<E> es;
        For(round, 4) {
            For(RNG(0, 5)) {
                es.emplace_back(RNG(0, n), RNG(0, n), RNG(0, 6));
                auto [a, b, w] = es.back();
                F.add(a, b, w);
            }
            long long f = F.flow();
            assert(f == brute_cut(n, es, s, t));
            auto [val, side] = F.cut();
            assert(val == f && side[s] == 0 && side[t] == 1);
            long long c = 0;
            for (auto [a, b, w]: es) if (a != b && side[a] == 0 && side[b] == 1) c += w;
            assert(c == f);
        }
    }
}

void test_gomory_hu() {
    For(iter, 500) {
        int n = RNG(2, 8), m = RNG(0, 12);
        Graph<long long, 0> G(n);
        vector<E> es;
        For(m) {
            int a = RNG(0, n), b = RNG(0, n);
            long long w = RNG(0, 6);
            if (a == b) continue;
            G.add(a, b, w);
            es.emplace_back(a, b, w), es.emplace_back(b, a, w);
        }
        G.build();
        auto T = gomory_hu_tree(G);
        assert(T.M == n - 1);
        For(s, n) For(t, s + 1, n) {
            // min edge weight on the tree path s - t
            vector<long long> mn(n, -1);
            mn[s] = linf;
            vector<int> st = {(int)s};
            while (!st.empty()) {
                int v = st.back(); st.pop_back();
                for (auto& e: T[v]) if (mn[e.to] == -1) mn[e.to] = min(mn[v], e.cost), st.push_back(e.to);
            }
            assert(mn[t] == brute_cut(n, es, s, t));
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_maxflow();
    test_change_before_flow();
    test_incremental();
    test_gomory_hu();
    solve();
    return 0;
}
