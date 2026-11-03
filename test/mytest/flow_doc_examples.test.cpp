#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/base.hpp"
#include "library/graph/connectivity/strongly_connected_component.hpp"
#include "library/graph/matching/dag_path_cover.hpp"
#include "library/graph/matching/maximum_antichain.hpp"
#include "library/flow/maxflow.hpp"
#include "library/flow/maxflow_with_lowerbound.hpp"
#include "library/flow/gomory_hu_tree.hpp"
#include "library/flow/bipartite.hpp"
#include "library/flow/rank_maximal_bipartite_matching.hpp"
#include "library/flow/mincostflow.hpp"
#include "library/flow/bflow.hpp"
#include "library/flow/dual_mcf.hpp"
#include "library/flow/longest_shortest_path.hpp"
#include "library/flow/hungarian.hpp"
#include "library/flow/unbalanced_transportation.hpp"
#include "library/flow/convex_min_cost_flow.hpp"
#include "library/flow/cyclic_b_flow.hpp"
#include "library/flow/min_cost_matching_on_line.hpp"
#include "library/flow/dynamic_mcf_on_line.hpp"
#include "library/flow/binary_optimization.hpp"
#include "library/flow/k_ary_optimization.hpp"

// every worked example in library/flow/theory, plus brute-force checks of the theorems quoted there

void chapter2_3() {
    MaxFlow<int> F(6, 0, 5);
    F.add(0, 1, 10), F.add(0, 2, 10), F.add(1, 2, 2), F.add(1, 3, 4), F.add(1, 4, 8);
    F.add(2, 4, 9), F.add(4, 3, 6), F.add(3, 5, 10), F.add(4, 5, 10);
    assert(F.flow() == 19);
    assert(F.cut().second == vector<int>({0, 1, 0, 1, 1, 1}));

    Graph<long long, 0> G(6);
    for (auto [a, b, c]: vector<tuple<int, int, int>>{{0, 1, 1}, {0, 3, 3}, {1, 2, 2}, {1, 4, 4}, {2, 5, 2}, {3, 4, 2}, {4, 5, 3}, {1, 3, 1}})
        G.add(a, b, c);
    G.build();
    auto T = gomory_hu_tree(G);
    vector<tuple<int, int, long long>> te;
    for (auto& e: T.edges) te.emplace_back(e.frm, e.to, e.cost);
    assert(te == (vector<tuple<int, int, long long>>{{3, 1, 4}, {1, 2, 4}, {0, 3, 4}, {1, 4, 8}, {4, 5, 5}}));
}

// edge in some / every min cut (chapter 3 §2), brute force over all cuts
void chapter3_min_cut_membership() {
    For(iter, 1500) {
        int n = RNG(2, 7), s = 0, t = n - 1, m = RNG(1, 10);
        vector<tuple<int, int, long long>> es;
        For(m) {
            int a = RNG(0, n), b = RNG(0, n);
            if (a != b) es.emplace_back(a, b, RNG(1, 4));
        }
        MaxFlow<long long> F(n, s, t);
        for (auto [a, b, c]: es) F.add(a, b, c);
        long long f = F.flow();
        vector<int> mins;  // all min cuts as masks of the source side
        For(mask, 1 << n) {
            if (!(mask >> s & 1) || (mask >> t & 1)) continue;
            long long c = 0;
            for (auto [a, b, w]: es) if ((mask >> a & 1) && !(mask >> b & 1)) c += w;
            assert(c >= f);
            if (c == f) mins.push_back(mask);
        }
        auto R = F.res_graph();
        auto [K, comp] = strongly_connected_component(R);
        auto reach = [&](int src, bool rev) {
            vector<int> vis(n);
            vector<int> st = {src};
            vis[src] = 1;
            while (!st.empty()) {
                int v = st.back(); st.pop_back();
                for (auto& e: R.edges) {
                    int a = rev ? e.to : e.frm, b = rev ? e.frm : e.to;
                    if (a == v && !vis[b]) vis[b] = 1, st.push_back(b);
                }
            }
            return vis;
        };
        auto from_s = reach(s, false), to_t = reach(t, true);
        For(i, len(es)) {
            bool sat = F.edges[F.pos[i]].cap == 0;
            auto [a, b, c] = es[i];
            bool some = false, every = true;
            for (int mask: mins) {
                bool in = (mask >> a & 1) && !(mask >> b & 1);
                some |= in, every &= in;
            }
            bool every_rule = from_s[a] && to_t[b];
            assert(every == every_rule);
            assert(some == (sat && comp[a] != comp[b]));
        }
    }
}

void chapter4() {
    MaxFlow_With_LowerBound<int> F(4, 0, 3);
    F.add(0, 1, 0, 3), F.add(0, 2, 0, 2), F.add(1, 2, 1, 2), F.add(1, 3, 0, 1), F.add(2, 3, 2, 4);
    assert(F.flow() == 5);
    assert(F.get_flow_result() == vector<int>({3, 2, 2, 1, 4}));
    MaxFlow_With_LowerBound<int> H(4, 0, 3);
    H.add(0, 1, 0, 3), H.add(0, 2, 0, 2), H.add(1, 2, 1, 2), H.add(1, 3, 0, 1), H.add(2, 3, 5, 6);
    assert(H.flow() == -1);

    // brute force: enumerate integral flows; self-loops; circulation with isolated s, t
    For(iter, 1500) {
        int n = RNG(2, 5), m = RNG(1, 5);
        bool circ = RNG(0, 2);
        int s = 0, t = n - 1;
        int N = n + (circ ? 2 : 0);
        if (circ) s = n, t = n + 1;
        vector<tuple<int, int, int, int>> es;
        For(m) {
            int lo = RNG(0, 3), hi = lo + RNG(0, 3);
            es.emplace_back(RNG(0, n), RNG(0, n), lo, hi);
        }
        MaxFlow_With_LowerBound<int> X(N, s, t);
        for (auto [a, b, lo, hi]: es) X.add(a, b, lo, hi);
        int best = -1;
        vector<int> f(m);
        auto rec = [&](auto& rec, int i) -> void {
            if (i == m) {
                vector<int> bal(N);
                For(j, m) bal[get<0>(es[j])] -= f[j], bal[get<1>(es[j])] += f[j];
                For(v, N) if (v != s && v != t && bal[v] != 0) return;
                chmax(best, bal[t]);
                return;
            }
            for (f[i] = get<2>(es[i]); f[i] <= get<3>(es[i]); ++f[i]) rec(rec, i + 1);
        };
        rec(rec, 0);
        int got = X.flow();
        assert(got == best);
        if (got != -1) {
            auto r = X.get_flow_result();
            vector<int> bal(N);
            For(j, m) {
                auto [a, b, lo, hi] = es[j];
                assert(lo <= r[j] && r[j] <= hi);
                bal[a] -= r[j], bal[b] += r[j];
            }
            For(v, N) if (v != s && v != t) assert(bal[v] == 0);
            assert(bal[t] == got);
        }
    }
}

void chapter5() {
    Graph<int, 1> G(5);
    G.add(0, 2), G.add(1, 2), G.add(2, 3), G.add(2, 4), G.build();
    assert(dag_path_cover(G) == vector<int>({1, 0, 1, 0, 1}));
    // Dilworth: #colors == max antichain on random DAGs (via transitive closure)
    For(iter, 500) {
        int n = RNG(1, 8);
        vector<vector<int>> reach(n, vector<int>(n));
        Graph<int, 1> D(n);
        For(a, n) For(b, a + 1, n) if (RNG(0, 3) == 0) D.add(a, b), reach[a][b] = 1;
        D.build();
        For(k, n) For(a, n) For(b, n) if (reach[a][k] && reach[k][b]) reach[a][b] = 1;
        auto col = dag_path_cover(D);
        int chains = *max_element(col.begin(), col.end()) + 1;
        For(a, n) For(b, a + 1, n) if (col[a] == col[b]) assert(reach[a][b]);
        int width = 0;
        For(mask, 1 << n) {
            bool ok = true;
            For(a, n) For(b, n) if ((mask >> a & 1) && (mask >> b & 1) && reach[a][b]) ok = false;
            if (ok) chmax(width, __builtin_popcount(mask));
        }
        assert(chains == width);
        Graph<int, 1> C(n);
        For(a, n) For(b, n) if (reach[a][b]) C.add(a, b);
        C.build();
        assert(len(maximum_antichain(C)) == width);
    }
}

void chapter6() {
    Graph<int, 0> G(8);
    for (auto [a, b]: vector<pair<int, int>>{{0, 4}, {0, 5}, {1, 4}, {2, 5}, {2, 6}, {2, 7}, {3, 5}}) G.add(a, b);
    G.build();
    BipartiteMatching<decltype(G)> B(G);
    assert(B.matching() == (vector<pair<int, int>>{{0, 4}, {2, 6}, {3, 5}}));
    assert(B.vertex_cover() == vector<int>({2, 4, 5}));
    assert(B.independent_set() == vector<int>({0, 1, 3, 6, 7}));
    assert(B.edge_cover() == vector<int>({0, 2, 4, 5, 6}));
    auto [K, W] = B.DM_decomposition();
    assert(K == 1 && W == vector<int>({0, 0, 1, 0, 0, 0, 1, 1}));

    Graph<int, 0> H(4);
    H.add(0, 2, 1), H.add(0, 3, 0), H.add(1, 2, 0), H.build();
    Rank_Maximal_Bipartite_Matching<decltype(H)> R(H);
    assert(R.match == vector<int>({2, -1, 0, -1}));

    // DM table: edge in some max matching ⇔ same class; vertex always matched rules
    For(iter, 1500) {
        int n1 = RNG(1, 5), n2 = RNG(1, 5), m = RNG(0, 9);
        Graph<int, 0> X(n1 + n2);
        vector<pair<int, int>> es;
        For(m) {
            int a = RNG(0, n1), b = n1 + RNG(0, n2);
            X.add(a, b), es.emplace_back(a, b);
        }
        X.build();
        BipartiteMatching<decltype(X)> M(X);
        int mx = len(M.matching());
        vector<int> in_some(m), in_every(m, 1), always(n1 + n2, 1);
        For(mask, 1 << m) {
            if (__builtin_popcount(mask) != mx) continue;
            vector<int> used(n1 + n2);
            bool ok = true;
            For(i, m) if (mask >> i & 1) {
                auto [a, b] = es[i];
                if (used[a]++ || used[b]++) ok = false;
            }
            if (!ok) continue;
            For(i, m) if (mask >> i & 1) in_some[i] = 1;
            For(i, m) if (!(mask >> i & 1)) in_every[i] = 0;
            For(v, n1 + n2) if (!used[v]) always[v] = 0;
        }
        auto [KK, WW] = M.DM_decomposition();
        For(i, m) {
            auto [a, b] = es[i];
            assert(in_some[i] == (WW[a] == WW[b]));
            int same = 0;  // edges inside the class of this edge
            for (auto [c, d]: es) same += (WW[c] == WW[a] && WW[d] == WW[a]);
            assert(in_every[i] == (WW[a] == WW[b] && same == 1));
        }
        For(v, n1 + n2) {
            bool rule = (M.color[v] == 0 ? (1 <= WW[v] && WW[v] <= KK) : (0 <= WW[v] && WW[v] <= KK - 1));
            assert(always[v] == rule);
        }
    }
}

void chapter7_8() {
    {
        Min_Cost_Flow<int, long long> G(4, 0, 3);
        G.add(0, 1, 2, 1), G.add(0, 2, 2, 4), G.add(1, 2, 1, 1), G.add(1, 3, 1, 5), G.add(2, 3, 3, 1);
        assert(G.slope() == (vector<pair<int, long long>>{{0, 0}, {1, 3}, {3, 13}, {4, 19}}));
        assert(G.get_potentials() == vector<long long>({-6, -5, -1, 0}));
        assert(G.path_decomposition() == (vector<vector<int>>{{0, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2, 3}}));
        Min_Cost_Flow<int, long long> H(4, 0, 3);
        H.add(0, 1, 2, 1), H.add(0, 2, 2, 4), H.add(1, 2, 1, 1), H.add(1, 3, 1, 5), H.add(2, 3, 3, 1);
        assert(H.flow(2) == make_pair(2, 8LL));
    }
    {
        MinCostFlow<long long, long long> B(5);
        B.add_source(0, 3), B.add_source(1, 2), B.add_sink(2, 1), B.add_sink(4, 4);
        B.add(0, 2, 0, 3, 2), B.add(0, 3, 0, 2, 1), B.add(1, 3, 0, 3, 1);
        B.add(2, 4, 1, 3, 1), B.add(3, 4, 0, 4, 3), B.add(1, 2, 0, 2, 3);
        auto [ok, v] = B.solve();
        assert(ok && v == 16);
        vector<long long> fl;
        for (auto& e: B.get_edges()) fl.push_back(e.flow());
        assert(fl == vector<long long>({3, 0, 2, 2, 2, 0}));
    }
    {
        Dual_MCF D(3);
        D.add_b(0, 1), D.add_b(2, -1);
        D.add_constraint(0, 1, 3), D.add_constraint(1, 2, 2), D.add_constraint(0, 2, 4);
        auto [val, P] = D.solve();
        assert(val == -4 && P == vector<long long>({-4, -1, 0}));
    }
    {
        Longest_Shortest_Path<long long> X(3, 0, 2);
        X.add(0, 1, 1, 1), X.add(1, 2, 1, 2), X.add(0, 2, 3, 1);
        assert(X.init_dist() == 2);
        assert(X.solve_by_target_length(4) == 3);
        assert(X.get_edges() == vector<long long>({3, 1, 4}));
        Longest_Shortest_Path<long long> Y(3, 0, 2);
        Y.add(0, 1, 1, 1), Y.add(1, 2, 1, 2), Y.add(0, 2, 3, 1);
        assert(Y.solve_by_cost(5) == 5);
    }
}

void chapter9() {
    vector<vector<long long>> C = {{4, 1, 3, 9}, {2, 0, 5, 7}, {3, 2, 2, 4}};
    auto [ans, match, X, Y] = hungarian<long long, true>(C);
    assert(ans == 5 && match == vector<int>({1, 0, 2}));
    assert(X == vector<long long>({3, 2, 2}) && Y == vector<long long>({0, -2, 0, 0}));

    vector<int> sup = {2, 1};
    Unbalanced_Transportation_Problem<long long> U(2, 3, sup);
    U.add(0, 0, 1), U.add(0, 1, 5), U.add(0, 2, 3), U.add(1, 0, 4), U.add(1, 1, 2), U.add(1, 2, 6);
    assert(U.calc() == vector<long long>({0, 1, 3, 6}));

    Convex_Min_Cost_Flow<int, long long> CV(2, 0, 1);
    CV.add_convex(0, 1, 3, [](int x) -> long long { return 1LL * x * x; });
    CV.add(0, 1, 2, 3);
    assert(CV.flow(3) == make_pair(3, 7LL));
    assert(len(CV.path_decomposition()) == 3);

    assert((cyclic_b_flow<long long, long long>({3, -1, -2, 0}, {1, 1, 1, 1}) == 5));
    assert(min_cost_matching_on_line({2, 0, 1, 0, 0, 0}, {0, 1, 0, 0, 2, 0}) == 7);

    Dynamic_MCF_on_Line<long long> D(3, {1, 1}, {1, 1});
    D.add_source(0, -5), D.add_sink(2, 2);
    assert(D.ANS == -3);
    D.add_sink(0, 1);
    assert(D.ANS == -4);
    D.rm_sink(0, 1);
    assert(D.ANS == -3);
}

void chapter10() {
    Binary_Optimization<long long, false> B(4);
    B.add_1(0, 0, 8), B.add_1(1, 0, 5), B.add_1(2, 0, -6), B.add_1(3, 0, -3);
    long long NG = -1000000000;
    B.add_2(0, 2, 0, 0, NG, 0), B.add_2(0, 3, 0, 0, NG, 0), B.add_2(1, 3, 0, 0, NG, 0);
    auto [v, x] = B.calc();
    assert(v == 4 && x == vector<int>({1, 1, 1, 1}));

    K_ary_Optimization<long long, true> K({3, 3});
    K.add_1(0, {3, 1, 4}), K.add_1(1, {0, 2, 1});
    vector<vector<long long>> c(3, vector<long long>(3));
    For(a, 3) For(b, 3) c[a][b] = 2 * abs(a - b);
    K.add_2(0, 1, c);
    auto [kv, kx] = K.calc();
    assert(kv == 3 && kx == vector<int>({0, 0}));
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    chapter2_3();
    chapter3_min_cut_membership();
    chapter4();
    chapter5();
    chapter6();
    chapter7_8();
    chapter9();
    chapter10();
    solve();
    return 0;
}
