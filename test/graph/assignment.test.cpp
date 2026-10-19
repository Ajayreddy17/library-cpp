#define PROBLEM "https://judge.yosupo.jp/problem/assignment"
#include <library/template.hpp>
#include <numeric>

#include <library/flow/hungarian.hpp>

void solve() {
    LL(N);
    VVT(long long, A, N, N);
    auto [ans, match, X, Y] = hungarian<long long, true>(A);
    print(ans);
    print(match);

    assert(accumulate(X.begin(), X.end(), 0LL) + accumulate(Y.begin(), Y.end(), 0LL) == ans);
    For(i, N) For(j, N) assert(X[i] + Y[j] <= A[i][j]);
}

signed main() {
    cout << fixed << setprecision(15);

    solve();

    return 0;
}
