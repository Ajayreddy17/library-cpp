#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree.hpp>

void solve() {
    LL(N, Q);
    FenwickTree<Monoid_Add<long long>> bit(N, [&](int i) -> long long {
        LL(x);
        return x;
    });

    For(Q) {
        LL(t, a, b);
        if (t == 0) {
            bit.add(a, b);
        } else {
            print(bit.prod(a, b));
        }
    }
}

signed main() {
    solve();
    return 0;
}
