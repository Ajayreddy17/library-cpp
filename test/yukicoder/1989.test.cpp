#define PROBLEM "https://yukicoder.me/problems/no/1989"
#include "library/template.hpp"
#include "library/mod/modint.hpp"

using mint = modint998;

void solve() {
    LL(N, M);
    mint avg = mint(M) * mint(N) / mint(N + N + 1);
    mint x = C_negative<mint, true>(N + N + 1, M);
    print(avg * x);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
