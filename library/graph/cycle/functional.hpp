#ifndef AJAY_FUNCTIONAL
#define AJAY_FUNCTIONAL

#include "library/graph/tree.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/monoid/monoid_add.hpp"
#include "library/monoid/monoid_pow.hpp"
#include "library/util/binary_serch.hpp"

namespace mitsuha{
// https://x.com/maspy_stars/status/1559842309688008707
// v -> TO[v] for every v. Internally a tree rooted at the extra vertex N:
// each component's root r (a cycle vertex) hangs from N, every other v from TO[v].
// Edge v -> TO[v] carries wt[v], an element of a group (needs Monoid::inverse).
// G and tree are members (tree refers to G), so the struct is not copyable.
template <typename Monoid = Monoid_Add<int>>
struct FunctionalGraph {
    using MX = Monoid;
    using X = typename MX::value_type;
    int N, M;
    vector<int> TO;
    vector<X> wt, dp; // dp[v] = wt product of the path v -> root[v]
    vector<int> root;
    vector<int> nearest_in_cycle; // first cycle vertex on the walk from v
    Graph<int, 1> G;
    Tree<Graph<int, 1>> tree;

    FunctionalGraph(int N) : N(N), M(0), TO(N, -1), wt(N, MX::unit()), root(N, -1), tree(G, -1) {}
    FunctionalGraph(const FunctionalGraph&) = delete;
    FunctionalGraph& operator=(const FunctionalGraph&) = delete;

    void add(int a, int b, X c = MX::unit()) {
        assert(0 <= a && a < N);
        assert(TO[a] == -1);
        ++M;
        TO[a] = b;
        wt[a] = c;
    }

    void build() {
        assert(N == M);
        UnionFind uf(N);
        For(v, N) if (!uf.merge(v, TO[v])) { root[v] = v; }
        For(v, N) if (root[v] == v) root[uf[v]] = v;
        For(v, N) root[v] = root[uf[v]];

        G.build(N + 1);
        For(v, N) {
            if (root[v] == v)
                G.add(N, v);
            else
                G.add(TO[v], v);
        }
        G.build();
        tree.build(N);
        dp.assign(N, MX::unit());
        For(i, 1, N + 1) {
            int v = tree.V[i];
            int p = tree.parent[v];
            if (p == N) continue;
            dp[v] = MX::op(wt[v], dp[p]);
        }
        nearest_in_cycle.assign(N, -1);
        For(i, 1, N + 1) {
            int v = tree.V[i];
            nearest_in_cycle[v] = (in_cycle(v) ? v : nearest_in_cycle[tree.parent[v]]);
        }
    }

    // steps from a to b, iinf if b is unreachable. O(1)
    int dist(int a, int b) {
        if (tree.in_subtree(a, b)) return tree.depth[a] - tree.depth[b];
        int r = root[a];
        int btm = TO[r];
        // a -> r -> btm -> b
        if (tree.in_subtree(btm, b)) {
            int x = tree.depth[a] - tree.depth[r];
            x += 1;
            x += tree.depth[btm] - tree.depth[b];
            return x;
        }
        return iinf;
    }

    // {F^step(v), product of wt along the walk}. O(log)
    pair<int, X> jump(int v, long long step) {
        int d = tree.depth[v];
        if (step <= d - 1) {
            int w = tree.jump(v, N, step);
            return {w, MX::op(dp[v], MX::inverse(dp[w]))};
        }
        X x = dp[v];
        v = root[v];
        step -= d - 1;
        int bottom = TO[v];
        int c = tree.depth[bottom];
        x = MX::op(x, monoid_pow<MX>(MX::op(wt[v], dp[bottom]), step / c));
        step %= c;
        if (step == 0) return {v, x};
        int w = tree.jump(bottom, N, step - 1);
        x = MX::op(x, wt[v]);
        x = MX::op(x, dp[bottom]);
        x = MX::op(x, MX::inverse(dp[w]));
        return {w, x};
    }

    // largest k with check(F^k(v), product of the first k weights), capped at linf.
    // check must be monotone (true, ..., true, false, ...) and check(v, unit) true.
    template <typename F>
    long long max_jump(F check, int v) {
        X prod = MX::unit();
        assert(check(v, prod));
        long long ans = 0;
        if (check(root[v], dp[v])) {
            ans += tree.depth[v] - 1, prod = dp[v], v = root[v];
            int bottom = TO[v];
            long long c = tree.depth[bottom];
            vector<X> pw;
            pw.emplace_back(MX::op(wt[v], dp[bottom]));
            For(k, 63) {
                if (!check(v, MX::op(prod, pw[k]))) break;
                if (ans + (c << k) >= linf) return linf;
                pw.emplace_back(MX::op(pw.back(), pw.back()));
            }
            Frr(k, len(pw)) {
                if (check(v, MX::op(prod, pw[k]))) {
                    ans += c << k;
                    prod = MX::op(prod, pw[k]);
                }
            }
            if (!check(bottom, MX::op(prod, wt[v]))) return ans;
            prod = MX::op(prod, wt[v]), v = bottom, ++ans;
        }
        auto pd = tree.get_path_decomposition(v, root[v], false);
        auto mycheck = [&](int w) -> bool {
            X x = MX::op(prod, MX::op(dp[v], MX::inverse(dp[w])));
            return check(w, x);
        };
        int last = v;
        for (auto [a, b]: pd) {
            swap(a, b);
            assert(a <= b);
            if (mycheck(tree.V[a])) {
                last = tree.V[a];
                continue;
            }
            if (!mycheck(tree.V[b])) break;
            int k = binary_search<FirstTrue>(a, b, [&](int i) -> bool { return mycheck(tree.V[i]); });
            last = tree.V[k];
            break;
        }
        ans += tree.depth[v] - tree.depth[last];
        return ans;
    }

    // F^step(v) for every v. O(N)
    vector<int> jump_all(long long step) {
        vector<int> res(N, -1);
        // the k-th ancestor of v goes to res[w]
        vector<vector<pair<int, int>>> query(N);
        For(v, N) {
            int d = tree.depth[v];
            int r = root[v];
            if (d - 1 > step) { query[v].emplace_back(v, step); }
            if (d - 1 <= step) {
                long long k = step - (d - 1);
                int bottom = TO[r];
                int c = tree.depth[bottom];
                k %= c;
                if (k == 0) {
                    res[v] = r;
                    continue;
                }
                query[bottom].emplace_back(v, k - 1);
            }
        }

        vector<int> path;
        auto dfs = [&](auto& dfs, int v) -> void {
            path.emplace_back(v);
            for (auto&& [w, k]: query[v]) { res[w] = path[len(path) - 1 - k]; }
            for (auto&& e: G[v]) dfs(dfs, e.to);
            path.pop_back();
        };
        for (auto&& e: G[N]) { dfs(dfs, e.to); }
        return res;
    }

    bool in_cycle(int v) {
        int r = root[v];
        int bottom = TO[r];
        return tree.in_subtree(bottom, v);
    }

    // cycle of root r in walking order: TO[r], TO[TO[r]], ..., r
    vector<int> collect_cycle(int r) {
        assert(r == root[r]);
        vector<int> cyc = {TO[r]};
        while (cyc.back() != r) cyc.emplace_back(TO[cyc.back()]);
        return cyc;
    }

    // smallest k such that F^k(i) = F^k(j),
    // or return -1 if no such k exist
    int meet_time(int i, int j) {
        if (i == j) return 0;
        if (root[i] != root[j]) return -1;
        int r = root[i];
        int b = TO[r];
        int n = tree.depth[b] - tree.depth[r] + 1; // cyc len
        if ((tree.depth[i] - tree.depth[j]) % n != 0) return -1;
        if (tree.depth[i] == tree.depth[j]) {
            int lca = tree.lca(i, j);
            return tree.depth[i] - tree.depth[lca];
        }
        int ti = tree.depth[i] - tree.depth[tree.lca(b, i)];
        int tj = tree.depth[j] - tree.depth[tree.lca(b, j)];
        return max(ti, tj);
    }
};
} // namespace mitsuha
#endif // AJAY_FUNCTIONAL
