#define PROBLEM "https://judge.yosupo.jp/problem/assignment"
#include <library/template.hpp>

#include <library/flow/hungarian.hpp>

void solve() {
    LL(N);
    VV(long long, A, N, N);
    auto [ans, match, X, Y] = hungarian<long long, true>(A);
    print(ans);
    print(match);

    assert(SUM<long long>(X) + SUM<long long>(Y) == ans);
    For(i, N) For(j, N) assert(X[i] + Y[j] <= A[i][j]);
}

signed main() {
    cout << fixed << setprecision(15);

    solve();

    return 0;
}
