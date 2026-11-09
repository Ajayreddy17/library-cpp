#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree.hpp"
#include "library/graph/datastructure/static_toptree.hpp"
#include "library/graph/datastructure/dynamic_tree_dp.hpp"
#include "library/graph/datastructure/dynamic_rerooting_tree_dp.hpp"
#include "library/graph/datastructure/distance_sum.hpp"
#include "library/mod/modint.hpp"

using mint = modint998;

// parent of i (i >= 1) is par[i] < i; vertex 0 is the root
vector<int> random_parents(int N) {
    vector<int> par(N, -1);
    int type = RNG(0, 5);
    For(i, 1, N) {
        if (type == 0) par[i] = RNG(0, i);
        if (type == 1) par[i] = i - 1;
        if (type == 2) par[i] = 0;
        if (type == 3) par[i] = (i - 1) / 2;
        if (type == 4) par[i] = (i % 3 == 0 ? i - 1 : max<int>(0, i - 3));
    }
    return par;
}

Graph<long long, 0> make_graph(const vector<int>& par, const vector<long long>& wt = {}) {
    int N = len(par);
    Graph<long long, 0> G(N);
    For(i, 1, N) G.add(par[i], i, wt.empty() ? 1 : wt[i]);
    G.build();
    return G;
}

// ---------------------------------------------------------------- structure
struct Count_DP {
    using value_type = int;
    static int rake(int a, int b) { return a + b; }
    static int compress(int a, int b) { return a + b; }
};

void test_structure() {
    For(iter, 400) {
        int N = RNG(1, 60);
        auto par = random_parents(N);
        auto G = make_graph(par);
        Tree<decltype(G)> tree(G);
        Static_TopTree<decltype(tree)> STT(tree);
        int n = len(STT.par);
        assert(n == 2 * N - 1);
        assert(STT.par[n - 1] == -1 && STT.A[n - 1] == -1);
        // vertex set of every cluster; boundary A is outside, every vertex hangs from the set or A
        vector<vector<int>> S(n);
        For(k, n) {
            if (k < N) {
                S[k] = {(int)k};
                assert(STT.A[k] == tree.parent[k] && STT.B[k] == k);
                continue;
            }
            int l = STT.lch[k], r = STT.rch[k];
            assert(l < k && r < k && STT.par[l] == k && STT.par[r] == k);
            S[k] = S[l];
            S[k].insert(S[k].end(), S[r].begin(), S[r].end());
            if (STT.is_compress[k]) {
                // (A, B[l]] + (B[l], B[r]]
                assert(STT.B[l] == STT.A[r] && STT.A[k] == STT.A[l] && STT.B[k] == STT.B[r]);
            } else {
                // both hang from the same A; L keeps its lower boundary
                assert(STT.A[l] == STT.A[r] && STT.A[k] == STT.A[l] && STT.B[k] == STT.B[l]);
            }
            set<int> in(S[k].begin(), S[k].end());
            assert(len(in) == len(S[k]) && !in.count(STT.A[k]) && in.count(STT.B[k]));
            for (int v: S[k]) assert(in.count(tree.parent[v]) || tree.parent[v] == STT.A[k]);
        }
        assert(len(S[n - 1]) == N);
        assert(STT.tree_dp<Count_DP>([&](int) { return 1; }) == N);
    }
}

// ---------------------------------------------------------------- ABC351 G
// f(v) = A[v] (leaf), A[v] + prod f(children) otherwise; answer f(root)
struct Hash_DP {
    struct X {
        mint a, b;  // value sent to the top boundary = a * x + b, x = input at the lower boundary
    };
    using value_type = X;
    static X compress(const X& L, const X& R) { return {L.a * R.a, L.a * R.b + L.b}; }
    static X rake(const X& L, const X& R) { return {L.a * R.b, L.b * R.b}; }
};

mint hash_brute(const vector<int>& par, const vector<mint>& A) {
    int N = len(par);
    vector<mint> f(N), prod(N, 1);
    vector<int> has(N);
    Frr(v, N) {
        f[v] = A[v] + (has[v] ? prod[v] : mint(0));
        if (v) prod[par[v]] *= f[v], has[par[v]] = 1;
    }
    return f[0];
}

void test_abc351g() {
    // sample 1 of ABC351 G (0-indexed)
    {
        vector<int> par = {-1, 0, 0};
        vector<mint> A = {3, 5, 1};
        auto G = make_graph(par);
        Tree<decltype(G)> tree(G);
        Dynamic_Tree_DP<decltype(tree), Hash_DP> DP(tree, [&](int v) -> Hash_DP::X { return {1, A[v]}; });
        A[2] = 4, DP.set(2, {1, A[2]});
        assert(DP.prod_all().b == mint(23));
        A[1] = 1, DP.set(1, {1, A[1]});
        assert(DP.prod_all().b == mint(7));
    }
    For(iter, 300) {
        int N = RNG(1, 60);
        auto par = random_parents(N);
        auto G = make_graph(par);
        Tree<decltype(G)> tree(G);
        vector<mint> A(N);
        For(v, N) A[v] = RNG(0, 998244353);
        Dynamic_Tree_DP<decltype(tree), Hash_DP> DP(tree, [&](int v) -> Hash_DP::X { return {1, A[v]}; });
        assert(DP.prod_all().b == hash_brute(par, A));
        For(q, 50) {
            int v = RNG(0, N);
            A[v] = RNG(0, 998244353);
            DP.set(v, {1, A[v]});
            assert(DP.prod_all().b == hash_brute(par, A));
        }
    }
}

// ---------------------------------------------------------------- ABC460 G
// answer(v) = total weight of the component of v among vertices of v's color
// per color c, with root r: g(u) = [C_u == c] (W_u + sum of g(children))
struct Flip_DP {
    struct X {
        int type, s, t;
        // type 0: value to s = a * x + b, x = input arriving at the lower boundary t
        // type 1: g(s) = a * y + b with y = value arriving from t's side,
        //         k = [C_s == c], r = pending amount for t's own sum
        long long a[2], b[2], k[2], r[2];
    };
    using value_type = X;
    static X rake(const X& L, const X& R) {
        assert(L.type == 0 && R.type == 0 && L.s == R.s);
        X x = {0, L.s, L.t};
        For(c, 2) x.a[c] = L.a[c], x.b[c] = L.b[c] + R.b[c], x.k[c] = x.r[c] = 0;
        return x;
    }
    static X rake2(const X& L, const X& R) {
        assert(L.type == 1 && R.type == 0 && L.s == R.s);
        X x = L;
        For(c, 2) x.b[c] = L.b[c] + L.k[c] * R.b[c];
        return x;
    }
    static X compress(const X& L, const X& R) {
        assert(L.type == 0 && R.type == 0 && L.t == R.s);
        X x = {0, L.s, R.t};
        For(c, 2) x.a[c] = L.a[c] * R.a[c], x.b[c] = L.a[c] * R.b[c] + L.b[c], x.k[c] = x.r[c] = 0;
        return x;
    }
    static X compress2(const X& L, const X& R) {
        assert(L.type == 1 && R.type == 1 && L.t == R.s);
        X x = {1, L.s, R.t};
        For(c, 2) {
            x.a[c] = L.a[c] * R.a[c];
            x.b[c] = L.a[c] * (R.b[c] + R.k[c] * L.r[c]) + L.b[c];
            x.k[c] = L.k[c], x.r[c] = R.r[c];
        }
        return x;
    }
    static X compress3(const X& L, const X& R) {
        assert(L.type == 1 && R.type == 0 && L.t == R.s);
        X x = L;
        For(c, 2) x.r[c] = L.r[c] + R.b[c];
        return x;
    }
};

void test_abc460g() {
    auto run = [&](const vector<int>& par, vector<long long> W, vector<int> C, bool sample) {
        int N = len(par);
        auto G = make_graph(par);
        Tree<decltype(G)> tree(G);
        auto single = [&](int v) -> pair<Flip_DP::X, Flip_DP::X> {
            int p = tree.parent[v];
            Flip_DP::X x0 = {0, p, v}, x1 = {1, v, p};
            For(c, 2) {
                long long g = (C[v] == c);
                x0.a[c] = g, x0.b[c] = g * W[v], x0.k[c] = x0.r[c] = 0;
                x1.a[c] = g, x1.b[c] = g * W[v], x1.k[c] = g, x1.r[c] = 0;
            }
            return {x0, x1};
        };
        Dynamic_Rerooting_Tree_DP<decltype(tree), Flip_DP> DP(tree, single);
        auto brute = [&](int v) -> long long {
            vector<int> vis(N);
            vector<int> st = {v};
            vis[v] = 1;
            long long s = 0;
            while (len(st)) {
                int x = st.back();
                st.pop_back();
                s += W[x];
                for (auto& e: G[x]) if (!vis[e.to] && C[e.to] == C[v]) vis[e.to] = 1, st.emplace_back(e.to);
            }
            return s;
        };
        auto query = [&](int v) {
            long long got = DP.prod_all(v).b[C[v]];
            assert(got == brute(v));
            return got;
        };
        if (sample) {
            // ABC460 G sample 1 (0-indexed): 11111 1 10 1100 10012
            vector<long long> out;
            out.emplace_back(query(0));
            C[1] ^= 1, DP.set(1, single(1));
            out.emplace_back(query(0)), out.emplace_back(query(1)), out.emplace_back(query(2));
            C[2] ^= 1, DP.set(2, single(2));
            C[1] ^= 1, DP.set(1, single(1));
            W[0] += 1, DP.set(0, single(0));
            out.emplace_back(query(4));
            assert(out == vector<long long>({11111, 1, 10, 1100, 10012}));
            return;
        }
        For(v, N) query(v);
        For(q, 60) {
            int t = RNG(0, 3), v = RNG(0, N);
            if (t == 0) C[v] ^= 1, DP.set(v, single(v));
            if (t == 1) W[v] += RNG(1, 100), DP.set(v, single(v));
            if (t == 2) query(v);
        }
        For(v, N) query(v);
    };
    // sample: edges 1-2, 2-3, 3-4, 2-5 rooted at 1
    run({-1, 0, 1, 2, 1}, {1, 10, 100, 1000, 10000}, {0, 0, 0, 0, 0}, true);
    For(iter, 300) {
        int N = RNG(1, 50);
        auto par = random_parents(N);
        vector<long long> W(N);
        vector<int> C(N);
        For(v, N) W[v] = RNG(1, 1000), C[v] = RNG(0, 2);
        run(par, W, C, false);
    }
}

// ---------------------------------------------------------------- Distance_Sum
void test_distance_sum() {
    For(iter, 300) {
        int N = RNG(1, 40);
        auto par = random_parents(N);
        vector<long long> wt(N);
        For(i, N) wt[i] = RNG(1, 10);
        auto G = make_graph(par, wt);
        Tree<decltype(G)> tree(G);
        vector<vector<long long>> D(N, vector<long long>(N));
        For(s, N) For(t, N) D[s][t] = tree.depth_weighted[s] + tree.depth_weighted[t] - 2 * tree.depth_weighted[tree.lca(s, t)];
        Distance_Sum<decltype(tree), long long> DS(tree);
        vector<long long> cnt(N);
        For(q, 60) {
            int v = RNG(0, N);
            if (RNG(0, 2)) {
                long long x = RNG(0, 5);
                DS.set(v, x), cnt[v] = x;
            } else {
                long long x = RNG(1, 5);
                DS.add(v, x), cnt[v] += x;
            }
            long long all = 0;
            For(a, N) For(b, a + 1, N) all += cnt[a] * cnt[b] * D[a][b];
            assert(DS.all_pairs_sum() == all);
            int u = RNG(0, N);
            long long from = 0;
            For(a, N) from += cnt[a] * D[u][a];
            assert(DS.query(u) == from);
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_structure();
    test_abc351g();
    test_abc460g();
    test_distance_sum();
    solve();
    return 0;
}
