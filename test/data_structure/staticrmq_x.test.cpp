#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include <library/template.hpp>

#include <library/datastructure/sparse_table/static_rmq.hpp>
#include <library/monoid/monoid_min.hpp>

void solve() {
    INT(N, Q);
    Static_RMQ<Monoid_Min<int>> seg(N, [&](int i) -> int {
        INT(x);
        return x;
    });
    For(Q) {
        INT(l, r);
        print(seg.prod(l, r));
    }
}

signed main() {
    solve();
    return 0;
}
