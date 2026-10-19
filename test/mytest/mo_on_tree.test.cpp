#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/random/base.hpp>
#include <library/graph/tree.hpp>
#include <library/graph/datastructure/mo_on_tree.hpp>
#include <library/graph/datastructure/tree_monoid.hpp>
#include <library/monoid/monoid_affine.hpp>
#include <library/monoid/monoid_add_pair.hpp>
#include <library/mod/modint.hpp>

using mint = modint998;
using AFF = pair<mint, mint>;

AFF gen() {
    mint a = RNG(1, 3);
    mint b = RNG(0, 3);
    return {a, b};
}

template <typename Mono, bool EDGE>
void test() {
    constexpr bool ORIENTED = !(Mono::commute);
    For(N, 1, 50) {
        For(Q, 1, 50) {
            vector<pair<int, int>> query(Q);
            vector<AFF> dat;
            if (!EDGE) {
                For(v, N) dat.emplace_back(gen());
            } else {
                For(i, N - 1) dat.emplace_back(gen());
            }
            Graph<int, 0> G(N);
            For(v, 1, N) {
                int p = RNG(0, v);
                G.add(p, v);
            }
            G.build();
            Tree<decltype(G)> tree(G);
            Tree_Monoid<decltype(tree), Mono, EDGE> TM(tree, dat);

            For(q, Q) {
                int a = RNG(0, N);
                int b = RNG(0, N);
                query[q] = {a, b};
            }
            Mo_on_Tree<decltype(tree), ORIENTED> mo(tree);
            for (auto&& [a, b] : query) mo.add(a, b);

            if constexpr (!EDGE) {
                AFF f = Mono::unit();
                auto init = [&]() -> void { f = dat[0]; };
                auto add_l = [&](int v) -> void { f = Mono::op(dat[v], f); };
                auto rm_l = [&](int v) -> void {
                    f = Mono::op(Mono::inverse(dat[v]), f);
                };
                auto add_r = [&](int v) -> void { f = Mono::op(f, dat[v]); };
                auto rm_r = [&](int v) -> void {
                    f = Mono::op(f, Mono::inverse(dat[v]));
                };
                auto ans = [&](int q) -> void {
                    assert(f == TM.prod_path(query[q].first, query[q].second));
                };
                mo.calc_vertex(init, add_l, add_r, rm_l, rm_r, ans);
            } else {
                AFF f = Mono::unit();
                auto get = [&](int a, int b) -> int {
                    return tree.v_to_e((tree.parent[a] == b ? a : b));
                };
                auto init = [&]() -> void {};
                auto add_l = [&](int a, int b) -> void {
                    f = Mono::op(dat[get(a, b)], f);
                };
                auto rm_l = [&](int a, int b) -> void {
                    f = Mono::op(Mono::inverse(dat[get(a, b)]), f);
                };
                auto add_r = [&](int a, int b) -> void {
                    f = Mono::op(f, dat[get(a, b)]);
                };
                auto rm_r = [&](int a, int b) -> void {
                    f = Mono::op(f, Mono::inverse(dat[get(a, b)]));
                };
                auto ans = [&](int q) -> void {
                    assert(f == TM.prod_path(query[q].first, query[q].second));
                };
                mo.calc_edge(init, add_l, add_r, rm_l, rm_r, ans);
            }
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    // パスの向きが可逆で頂点可換モノイド積
    test<Monoid_Add_Pair<mint>, false>();
    // パスの向きが不可逆で頂点非可換モノイド積
    test<Monoid_Affine<mint>, false>();
    // パスの向きが可逆で辺可換モノイド積
    test<Monoid_Add_Pair<mint>, true>();
    // パスの向きが不可逆で辺非可換モノイド積
    test<Monoid_Affine<mint>, true>();
    solve();
    return 0;
}
