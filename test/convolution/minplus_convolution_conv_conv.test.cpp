#define PROBLEM \
    "https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex"
#include <library/template.hpp>

#include <library/convex/minplus_convolution.hpp>

void solve() {
    LL(N, M);
    VT(long long, A, N);
    VT(long long, B, M);
    print(minplus_convolution<long long, 1, 1>(A, B));
}

signed main() {
    solve();
    return 0;
}
