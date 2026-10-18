#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/datastructure/static_range_frequency.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    Static_Range_Frequency X(A);
    For(Q) {
        INT(l, r, x);
        print(X.query(l, r, x));
    }
}

signed main() {
    solve();
    return 0;
}
