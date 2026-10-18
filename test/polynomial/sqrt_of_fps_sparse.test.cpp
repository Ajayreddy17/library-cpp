#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse"
#include <library/template.hpp>

#include <library/poly/fps_sqrt.hpp>

using mint = modint998;

void solve() {
    LL(N, K);
    vector<mint> f(N);
    For(K) {
        LL(i, a);
        f[i] = a;
    }
    f = fps_sqrt_any(f);
    if (len(f) == 0) return print(-1);
    print(f);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
