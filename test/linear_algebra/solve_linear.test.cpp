#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include <library/template.hpp>
#include <library/mod/modint.hpp>
#include <library/linear_algebra/solve_linear.hpp>
#include <library/linear_algebra/matrix_rank.hpp>

using mint = modint998;
void solve() {
    LL(N, M);
    VV(mint, A, N, M);
    VT(mint, b, N);
    auto xs = solve_linear(A, b);
    if (len(xs) == 0) return print(-1);

    assert(len(xs) - 1 == M - matrix_rank(A));

    print(len(xs) - 1);
    For(r, len(xs)) print(xs[r]);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
