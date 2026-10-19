#define PROBLEM "https://judge.yosupo.jp/problem/persistent_queue"
#include <library/template.hpp>

#include <library/datastructure/dynamic_array.hpp>

void solve() {
    LL(Q);
    vector<int> L, R;

    Dynamic_Array<int, true> X(2000000, 0);
    using np = typename decltype(X)::np;
    vector<np> roots;

    roots.emplace_back(X.new_root());
    L.emplace_back(0), R.emplace_back(0);

    For(Q) {
        LL(t, k);
        ++k;
        np root = roots[k];
        int l = L[k], r = R[k];

        if (t == 0) {
            INT(x);
            root = X.set(root, r++, x);
        }
        if (t == 1) {
            print(X.get(root, l++));
        }
        roots.emplace_back(root);
        L.emplace_back(l), R.emplace_back(r);
    }
}

signed main() {
    solve();
    return 0;
}
