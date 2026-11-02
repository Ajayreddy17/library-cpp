#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/flow/mincostflow.hpp"
#include "library/flow/bflow.hpp"
#include "library/flow/convex_min_cost_flow.hpp"
#include "library/flow/cyclic_b_flow.hpp"
#include "library/flow/unbalanced_transportation.hpp"
#include "library/flow/dynamic_mcf_on_line.hpp"
#include "library/flow/longest_shortest_path.hpp"

// potentials: reduced cost >= 0 on residual edges between vertices reachable from source
void test_potentials() {
    For(iter, 1000) {
        int n = RNG(2, 7), s = 0, t = n - 1;
        Min_Cost_Flow<int, long long> G(n, s, t);
        For(RNG(0, 12)) G.add(RNG(0, n), RNG(0, n), RNG(0, 4), RNG(0, 10));
        G.flow();
        auto p = G.get_potentials();
        auto es = G.edges();
        vector<int> vis(n);
        vis[s] = 1;
        For(n) for (auto& e: es) {
            if (e.cap > e.flow && vis[e.frm]) vis[e.to] = 1;
            if (e.flow > 0 && vis[e.to]) vis[e.frm] = 1;
        }
        for (auto& e: es) {
            if (!vis[e.frm] || !vis[e.to]) continue;
            if (e.cap > e.flow) assert(e.cost + p[e.frm] - p[e.to] >= 0);
            if (e.flow > 0) assert(-e.cost + p[e.to] - p[e.frm] >= 0);
        }
    }
}

// convex edge f(x) = a x + b x^2 == unit edges with costs f(k) - f(k - 1)
void test_convex() {
    For(iter, 1000) {
        int n = RNG(2, 6), s = 0, t = n - 1;
        Convex_Min_Cost_Flow<int, long long> C(n, s, t);
        Min_Cost_Flow<int, long long> G(n, s, t);
        For(RNG(0, 8)) {
            int a = RNG(0, n), b = RNG(0, n);
            if (a == b) continue;
            int cap = RNG(0, 4);
            long long p = RNG(0, 5), q = RNG(0, 3);
            C.add_convex(a, b, cap, [&](int x) -> long long { return p * x + q * x * x; });
            For(k, 1, cap + 1) G.add(a, b, 1, p + q * (2 * k - 1));
        }
        int lim = RNG(0, 8);
        assert(C.flow(lim) == G.flow(lim));
    }
}

// cyclic b-flow == b-flow on the bidirected cycle
void test_cyclic() {
    For(iter, 1000) {
        int n = RNG(1, 7);
        vector<long long> sup(n), cost(n);
        For(i, n) sup[i] = RNG(-5, 6), cost[i] = RNG(0, 6);
        long long sum = accumulate(sup.begin(), sup.end(), 0LL);
        sup[0] -= sum;
        long long got = cyclic_b_flow<long long, long long>(sup, cost);
        if (n == 1) { assert(got == 0); continue; }
        MinCostFlow<long long, long long> B(n);
        For(i, n) {
            int j = (i + 1) % n;
            B.add(i, j, 0, 1000, cost[i]);
            B.add(j, i, 0, 1000, cost[i]);
            B.add_source(i, sup[i]);
        }
        auto [ok, val] = B.solve();
        assert(ok && (long long)val == got);
    }
}

// unbalanced transportation: cost for each flow amount == fresh min cost flow
void test_unbalanced() {
    For(iter, 500) {
        int K = RNG(1, 4), N = RNG(1, 7);
        vector<int> sup(K);
        For(i, K) sup[i] = RNG(0, 4);
        sup[0] += max(0, N - accumulate(sup.begin(), sup.end(), 0));
        vector<vector<long long>> c(K, vector<long long>(N));
        Unbalanced_Transportation_Problem<long long> U(K, N, sup);
        For(a, K) For(b, N) c[a][b] = RNG(0, 10), U.add(a, b, c[a][b]);
        auto res = U.calc();
        assert(len(res) == N + 1);
        For(f, N + 1) {
            int S = K + N, T = S + 1;
            Min_Cost_Flow<int, long long> G(K + N + 2, S, T);
            For(a, K) G.add(S, a, sup[a], 0);
            For(a, K) For(b, N) G.add(a, K + b, 1, c[a][b]);
            For(b, N) G.add(K + b, T, 1, 0);
            auto [fl, cs] = G.flow(f);
            assert(fl == f && cs == res[f]);
        }
    }
}

// dynamic min cost flow on a line == min cost circulation
void test_dynamic_line() {
    For(iter, 300) {
        int n = RNG(1, 6);
        vector<int> rc(n - 1), lc(n - 1);
        For(i, n - 1) rc[i] = RNG(0, 3), lc[i] = RNG(0, 3);
        Dynamic_MCF_on_Line<long long> D(n, rc, lc);
        multiset<pair<int, long long>> src, snk;
        For(q, 20) {
            int typ = RNG(0, 4), v = RNG(0, n);
            long long c = RNG(-6, 7);
            if (typ == 0) D.add_source(v, c), src.emplace(v, c);
            if (typ == 1) D.add_sink(v, c), snk.emplace(v, c);
            if (typ == 2 && !src.empty()) {
                auto it = next(src.begin(), RNG(0, len(src)));
                D.rm_source(it->first, it->second), src.erase(it);
            }
            if (typ == 3 && !snk.empty()) {
                auto it = next(snk.begin(), RNG(0, len(snk)));
                D.rm_sink(it->first, it->second), snk.erase(it);
            }
            int S = n, T = n + 1;
            MinCostFlow<long long, long long> B(n + 2);
            For(i, n - 1) B.add(i, i + 1, 0, rc[i], 0), B.add(i + 1, i, 0, lc[i], 0);
            for (auto [v, c]: src) B.add(S, v, 0, 1, c);
            for (auto [v, c]: snk) B.add(v, T, 0, 1, c);
            B.add(T, S, 0, 100, 0);
            auto [ok, val] = B.solve();
            assert(ok && (long long)val == D.ANS);
        }
    }
}

// longest shortest path vs brute force over extensions
void test_longest_shortest_path() {
    For(iter, 300) {
        int n = RNG(2, 5), m = RNG(1, 5), s = 0, t = n - 1;
        vector<tuple<int, int, long long, long long>> es;
        es.emplace_back(s, t, RNG(0, 4), RNG(1, 4));  // keep t reachable
        For(m - 1) es.emplace_back(RNG(0, n), RNG(0, n), RNG(0, 4), RNG(1, 4));
        auto dist = [&](const vector<long long>& ext) {
            vector<long long> d(n, linf);
            d[s] = 0;
            For(n) For(i, len(es)) {
                auto [a, b, l, c] = es[i];
                if (d[a] < linf) chmin(d[b], d[a] + l + ext[i]);
            }
            return d[t];
        };
        int M = len(es);
        long long D0 = dist(vector<long long>(M));
        // brute min cost for target L, extensions in [0, L]
        auto brute = [&](long long L) {
            long long best = linf;
            vector<long long> ext(M);
            auto rec = [&](auto& rec, int i, long long c) -> void {
                if (c >= best) return;
                if (i == M) { if (dist(ext) >= L) best = c; return; }
                For(x, L + 1) ext[i] = x, rec(rec, i + 1, c + x * get<3>(es[i]));
                ext[i] = 0;
            };
            rec(rec, 0, 0);
            return best;
        };
        long long L = D0 + RNG(0, 3);
        Longest_Shortest_Path<long long> X(n, s, t);
        for (auto [a, b, l, c]: es) X.add(a, b, l, c);
        long long K = X.solve_by_target_length(L);
        assert(K == brute(L));
        auto E = X.get_edges();
        vector<long long> ext(M);
        long long c = 0;
        For(i, M) ext[i] = E[i] - get<2>(es[i]), c += ext[i] * get<3>(es[i]), assert(ext[i] >= 0);
        assert(dist(ext) >= L && c == K);

        long long budget = RNG(0, 8);
        Longest_Shortest_Path<long long> Y(n, s, t);
        for (auto [a, b, l, c]: es) Y.add(a, b, l, c);
        long long best = D0;
        while (brute(best + 1) <= budget) ++best;
        assert(Y.solve_by_cost(budget) == best);
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_potentials();
    test_convex();
    test_cyclic();
    test_unbalanced();
    test_dynamic_line();
    test_longest_shortest_path();
    solve();
    return 0;
}
