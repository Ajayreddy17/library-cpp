#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/random/shuffle.hpp"
#include "library/graph/cycle/functional.hpp"
#include "library/graph/cycle/unicyclicgraph.hpp"

// permutations of {0,1,2,3}; op(x, y) = "x then y", non-commutative group
struct Monoid_Perm {
    using value_type = array<int, 4>;
    using X = value_type;
    static X op(const X &x, const X &y) {
        X z;
        For(i, 4) z[i] = y[x[i]];
        return z;
    }
    static X inverse(const X &x) {
        X z;
        For(i, 4) z[x[i]] = i;
        return z;
    }
    static X unit() { return {0, 1, 2, 3}; }
    static constexpr bool commute = false;
};

using P = Monoid_Perm::X;

P random_perm() {
    P p = Monoid_Perm::unit();
    For(i, 1, 4) swap(p[i], p[RNG(0, i + 1)]);
    return p;
}

void test_functional() {
    For(iter, 3000) {
        int N = RNG(1, 13);
        FunctionalGraph<Monoid_Perm> FG(N);
        vector<int> to(N);
        vector<P> w(N);
        For(v, N) to[v] = RNG(0, N), w[v] = random_perm(), FG.add(v, to[v], w[v]);
        FG.build();

        // walk[v][k] = F^k(v), prod[v][k] = product of the first k weights
        const int K = 3 * N + 2;
        vector<vector<int>> walk(N, vector<int>(K + 1));
        vector<vector<P>> prod(N, vector<P>(K + 1));
        For(v, N) {
            walk[v][0] = v, prod[v][0] = Monoid_Perm::unit();
            For(k, K) {
                int x = walk[v][k];
                walk[v][k + 1] = to[x];
                prod[v][k + 1] = Monoid_Perm::op(prod[v][k], w[x]);
            }
        }
        // cycle vertices: F^N(v) for some v
        vector<int> cyc(N);
        For(v, N) cyc[walk[v][N]] = 1;
        For(v, N) assert(FG.in_cycle(v) == bool(cyc[v]));
        For(v, N) {
            int x = v;
            while (!cyc[x]) x = to[x];
            assert(FG.nearest_in_cycle[v] == x);
        }
        For(v, N) if (FG.root[v] == v) {
            auto C = FG.collect_cycle(v);
            assert(C.back() == v);
            For(i, len(C)) assert(cyc[C[i]] && C[(i + 1) % len(C)] == to[C[i]]);
        }
        For(a, N) For(b, N) {
            int d = iinf;
            Frr(k, K + 1) if (walk[a][k] == b) d = k;
            assert(FG.dist(a, b) == d);
            int m = -1;
            Frr(k, K + 1) if (walk[a][k] == walk[b][k]) m = k;
            assert(FG.meet_time(a, b) == m);
        }
        For(v, N) For(k, K + 1) {
            auto [x, p] = FG.jump(v, k);
            assert(x == walk[v][k] && p == prod[v][k]);
        }
        // large step: reduce it with the period 12c (orders in S4 divide 12)
        For(v, N) {
            long long step = RNG(0, 1LL << 40);
            int x = v;
            P p = Monoid_Perm::unit();
            long long s = step;
            int c = len(FG.collect_cycle(FG.root[v]));
            long long period = 12LL * c;
            if (s > N) s = N + (s - N) % period;
            For(s) p = Monoid_Perm::op(p, w[x]), x = to[x];
            auto [y, q] = FG.jump(v, step);
            assert(y == x && q == p);
        }
        For(k, K + 1) {
            auto R = FG.jump_all(k);
            For(v, N) assert(R[v] == walk[v][k]);
        }
    }
    // default monoid: weights are additive
    For(iter, 300) {
        int N = RNG(1, 10);
        FunctionalGraph<> FG(N);
        vector<int> to(N), w(N);
        For(v, N) to[v] = RNG(0, N), w[v] = RNG(-5, 6), FG.add(v, to[v], w[v]);
        FG.build();
        For(v, N) For(k, 30) {
            int x = v, s = 0;
            For(k) s += w[x], x = to[x];
            auto [y, t] = FG.jump(v, k);
            assert(x == y && s == t);
        }
    }
    // max_jump: weights >= 0, check(x, s) = s <= B; also a vertex ban that holds from some step on
    For(iter, 3000) {
        int N = RNG(1, 12);
        FunctionalGraph<Monoid_Add<long long>> FG(N);
        vector<int> to(N);
        vector<long long> w(N);
        For(v, N) to[v] = RNG(0, N), w[v] = RNG(0, 2) ? RNG(0, 4) : 0, FG.add(v, to[v], w[v]);
        FG.build();
        For(v, N) {
            long long B = RNG(0, 40);
            auto check = [&](int, long long s) { return s <= B; };
            long long got = FG.max_jump(check, v);
            // brute: walk until the sum exceeds B; if a full cycle sums to 0 it never does
            int x = v;
            long long s = 0, k = 0, ans = -1;
            For(4 * N + 4 * 40 * N + 10) {
                if (s + w[x] > B) { ans = k; break; }
                s += w[x], x = to[x], ++k;
            }
            if (ans == -1) assert(got == linf);
            else assert(got == ans);
        }
    }
}

void test_unicyclic() {
    For(iter, 3000) {
        int N = RNG(1, 12);
        Graph<long long, 0> G(N);
        vector<int> perm(N);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm);
        vector<tuple<int, int, long long>> es;
        For(i, 1, N) es.emplace_back(perm[RNG(0, i)], perm[i], RNG(1, 10));
        es.emplace_back(RNG(0, N), RNG(0, N), RNG(1, 10)); // may be a self-loop or a parallel edge
        shuffle(es);
        for (auto [a, b, c]: es) G.add(a, b, c);
        G.build();
        UnicyclicGraph<decltype(G)> X(G);

        // brute: an edge is on the cycle iff removing it keeps the graph connected
        vector<int> on_cycle_edge(N), on_cycle_vertex(N);
        For(e, N) {
            UnionFind uf(N);
            For(f, N) if (f != e) uf.merge(G.edges[f].frm, G.edges[f].to);
            if (uf.n_comp == 1) on_cycle_edge[e] = 1, on_cycle_vertex[G.edges[e].frm] = on_cycle_vertex[G.edges[e].to] = 1;
        }
        For(v, N) assert(X.in_cycle[v] == bool(on_cycle_vertex[v]));
        assert(len(X.cycle) == accumulate(on_cycle_edge.begin(), on_cycle_edge.end(), 0));
        assert(on_cycle_edge[X.out_eid] && X.out_cost == G.edges[X.out_eid].cost);
        assert(X.cycle.back() == X.root && X.TO[X.root] == X.cycle[0]);
        For(v, N) if (v != X.root) assert(X.tree.parent[v] == X.TO[v]);
        assert(X.G.M == N - 1 && len(X.orig_eid) == N - 1);
        For(i, N - 1) {
            auto &e = X.G.edges[i];
            auto &f = G.edges[X.orig_eid[i]];
            assert(X.orig_eid[i] != X.out_eid && e.cost == f.cost);
            assert(make_pair(min(e.frm, e.to), max(e.frm, e.to)) == make_pair(min(f.frm, f.to), max(f.frm, f.to)));
            assert(X.tree.parent[e.to] == e.frm && X.tree.VtoE[e.to] == i);
        }

        // brute distances (Floyd–Warshall)
        vector<vector<long long>> D(N, vector<long long>(N, linf)), U(N, vector<long long>(N, linf));
        For(v, N) D[v][v] = U[v][v] = 0;
        for (auto &e: G.edges) {
            chmin(D[e.frm][e.to], e.cost), chmin(D[e.to][e.frm], e.cost);
            chmin(U[e.frm][e.to], 1LL), chmin(U[e.to][e.frm], 1LL);
            if (e.frm == e.to) D[e.frm][e.frm] = U[e.frm][e.frm] = 0;
        }
        For(k, N) For(i, N) For(j, N) chmin(D[i][j], D[i][k] + D[k][j]), chmin(U[i][j], U[i][k] + U[k][j]);
        For(a, N) For(b, N) {
            assert(X.dist(a, b) == U[a][b]);
            assert(X.dist_weighted(a, b) == D[a][b]);
        }
        // nearest cycle vertex: the unique closest one
        For(v, N) {
            int best = -1;
            For(c, N) if (on_cycle_vertex[c] && (best == -1 || U[v][c] < U[v][best])) best = c;
            For(c, N) if (on_cycle_vertex[c] && c != best) assert(U[v][c] > U[v][best]);
            assert(X.nearest_in_cycle[v] == best);
        }
    }
}

// every number quoted in graph/theory/functional_graph.md
void test_doc_examples() {
    {
        vector<int> to = {1, 2, 3, 1, 0, 0, 2, 6, 8, 8, 11, 10};
        vector<int> w = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8};
        int N = len(to);
        FunctionalGraph<> FG(N);
        For(v, N) FG.add(v, to[v], w[v]);
        FG.build();
        assert(FG.root == vector<int>({3, 3, 3, 3, 3, 3, 3, 3, 8, 8, 11, 11}));
        vector<int> depth(N);
        For(v, N) depth[v] = FG.tree.depth[v];
        assert(depth == vector<int>({4, 3, 2, 1, 5, 5, 3, 4, 1, 2, 2, 1}));
        assert(FG.dp == vector<int>({8, 5, 4, 0, 13, 17, 6, 12, 0, 3, 5, 0}));
        assert(FG.collect_cycle(3) == vector<int>({1, 2, 3}));
        assert(FG.collect_cycle(8) == vector<int>({8}));
        assert(FG.collect_cycle(11) == vector<int>({10, 11}));
        assert(FG.jump(7, 10) == make_pair(1, 25));
        assert(FG.jump_all(5) == vector<int>({2, 3, 1, 2, 1, 1, 3, 2, 8, 8, 11, 10}));
        assert(FG.dist(5, 3) == 4 && FG.dist(7, 1) == 4 && FG.dist(3, 5) == iinf);
        assert(FG.meet_time(5, 4) == 1 && FG.meet_time(9, 8) == 1 && FG.meet_time(4, 7) == -1);
        assert(FG.max_jump([](int, int s) { return s <= 20; }, 7) == 8);
        assert(FG.nearest_in_cycle == vector<int>({1, 1, 2, 3, 1, 1, 2, 2, 8, 8, 10, 11}));
        vector<int> tail(N);
        For(v, N) tail[v] = FG.tree.depth[v] - FG.tree.depth[FG.nearest_in_cycle[v]];
        assert(tail == vector<int>({1, 0, 0, 0, 2, 2, 1, 2, 0, 1, 0, 0}));
    }
    {
        vector<tuple<int, int, int>> es = {{0, 1, 2}, {1, 2, 3}, {2, 3, 1}, {3, 4, 4}, {4, 0, 2}, {1, 5, 1},
                                           {5, 6, 2}, {3, 7, 3}, {7, 8, 1}, {7, 9, 2}, {2, 10, 5}};
        Graph<int, 0> G(11);
        for (auto [a, b, c]: es) G.add(a, b, c);
        G.build();
        UnicyclicGraph X(G);
        assert(X.root == 4 && X.out_eid == 4 && X.out_cost == 2 && X.TO[4] == 0);
        assert(X.cycle == vector<int>({0, 1, 2, 3, 4}));
        assert(X.dist(6, 9) == 6 && X.dist_weighted(6, 9) == 12 && X.dist(8, 9) == 2);
        assert(X.nearest_in_cycle == vector<int>({0, 1, 2, 3, 4, 1, 1, 3, 3, 3, 2}));
    }
}

// maximum-weight independent set on a unicyclic graph (tree DP, fix the removed edge) vs brute force
void test_unicyclic_mis() {
    For(iter, 2000) {
        int N = RNG(1, 11);
        Graph<int, 0> G(N);
        vector<int> perm(N);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm);
        For(i, 1, N) G.add(perm[RNG(0, i)], perm[i]);
        G.add(RNG(0, N), RNG(0, N));
        G.build();
        vector<long long> A(N);
        For(v, N) A[v] = RNG(0, 10);

        UnicyclicGraph X(G);
        int r = X.root, s = X.TO[r];
        auto solve = [&](int banned) -> long long {
            vector<array<long long, 2>> dp(N);
            Frr(i, N) {
                int v = X.tree.V[i];
                dp[v] = {0, v == banned ? -linf : A[v]};
                for (auto&& e: X.G[v]) {
                    dp[v][0] += max(dp[e.to][0], dp[e.to][1]);
                    dp[v][1] += dp[e.to][0];
                }
            }
            return max(dp[r][0], dp[r][1]);
        };
        long long ans = max(solve(r), solve(s));

        long long best = 0;
        For(mask, 1 << N) {
            bool ok = true;
            for (auto& e: G.edges) if ((mask >> e.frm & 1) && (mask >> e.to & 1)) ok = false;
            if (!ok) continue;
            long long sum = 0;
            For(v, N) if (mask >> v & 1) sum += A[v];
            chmax(best, sum);
        }
        assert(ans == best);
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_functional();
    test_unicyclic();
    test_doc_examples();
    test_unicyclic_mis();
    solve();
    return 0;
}
