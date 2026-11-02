#define PROBLEM "https://yukicoder.me/problems/no/1212"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/datastructure/tree_monoid.hpp"
#include "library/monoid/monoid_min.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"

void solve() {
    INT(N);
    Graph<long long, 0> G(N);
    G.read_tree(1);

    Tree<decltype(G)> tree(G);

    LL(Q);
    vector<pair<long long, long long>> query(Q); read(query);
    for (auto&& [x, y] : query) --x, --y;
    vector<vector<int>> QIDS(N);
    For(q, Q) {
        auto [x, y] = query[q];
        int lca = tree.lca(x, y);
        QIDS[lca].emplace_back(q);
    }

    auto& par = tree.parent;

    using Mono = Monoid_Min<long long>;

    // 接続先 → 辺重み を管理するセグ木。
    // これを使って、dat[edge] = 親から見て edge 以外のところの min を作る
    SegTree<Mono> seg(N);
    vector<long long> dat(N - 1);
    vector<long long> dat_v(N);                  // 子方向の min
    vector<long long> dat_par(N, Mono::unit());  // 親方向

    auto build_at = [&](int v) -> void {
        for (auto&& e : G[v]) {
            if (e.to == par[v]) continue;
            seg.set(e.to, e.cost);
        }
    };
    auto reset_at = [&](int v) -> void {
        for (auto&& e : G[v]) {
            if (e.to == par[v]) continue;
            seg.set(e.to, Mono::unit());
        }
    };

    For(v, N) {
        build_at(v);
        dat_v[v] = seg.prod_all();
        for (auto&& e : G[v]) {
            if (e.to == par[v]) continue;
            dat_par[e.to] = e.cost;
            dat[e.id] = Mono::op(seg.prod(0, e.to), seg.prod(e.to + 1, N));
        }
        reset_at(v);
    }

    Tree_Monoid<decltype(tree), Mono, true> X(tree, dat);
    vector<long long> ANS(Q);
    For(lca, N) {
        build_at(lca);
        for (auto&& q : QIDS[lca]) {
            auto [S, T] = query[q];
            if (S > T) swap(S, T);
            long long base = tree.dist_weighted(S, T);
            long long best = Mono::unit();

            // S, T での寄り道
            if (S != lca) chmin(best, dat_v[S]);
            if (T != lca) chmin(best, dat_v[T]);
            // lca での寄り道
            chmin(best, dat_par[lca]);
            vector<int> ng = {-1, N};
            if (S != lca) ng.emplace_back(tree.jump(lca, S, 1));
            if (T != lca) ng.emplace_back(tree.jump(lca, T, 1));
            sort(ng.begin(), ng.end());
            For(k, len(ng) - 1) { chmin(best, seg.prod(ng[k] + 1, ng[k + 1])); }
            // S → lca の道中
            if (lca != S) {
                int v = tree.jump(lca, S, 1);
                chmin(best, X.prod_path(S, v));
            }
            if (lca != T) {
                int v = tree.jump(lca, T, 1);
                chmin(best, X.prod_path(T, v));
            }
            ANS[q] = (best == Mono::unit() ? -1 : base + 2 * best);
        }
        reset_at(lca);
    }
    for (auto&& x : ANS) print(x);
}

signed main() {
    solve();
    return 0;
}
