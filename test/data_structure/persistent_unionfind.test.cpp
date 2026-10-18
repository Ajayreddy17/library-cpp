#define PROBLEM "https://judge.yosupo.jp/problem/persistent_unionfind"
#include <library/template.hpp>


#include <library/datastructure/unionfind/dynamic_unionfind.hpp>

void solve() {
    LL(N, Q);

    Dynamic_UnionFind<true> uf(Q);
    using np = typename decltype(uf)::np;
    vector<np> roots;

    roots.emplace_back(uf.new_root());

    For(Q) {
        LL(t, k, u, v);
        ++k;
        auto root = roots[k];
        if (t == 0) {
            root = uf.merge(root, u, v).first;
        } else {
            bool ok = uf.root(root, u) == uf.root(root, v);
            print(ok ? 1 : 0);
        }
        roots.emplace_back(root);
    }
}

signed main() {
    solve();

    return 0;
}
