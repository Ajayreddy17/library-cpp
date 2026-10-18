#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include <library/template.hpp>
#include <library/datastructure/sparse_table/sparse_table.hpp>
#include <library/monoid/monoid_min.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);

    Sparse_Table<Monoid_Min<int>> seg(A);

    For(Q) {
        LL(a, b);
        print(seg.prod(a, b));
    }
}

signed main() {
    solve();
    return 0;
}
