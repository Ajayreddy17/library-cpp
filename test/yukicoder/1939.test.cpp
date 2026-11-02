#define PROBLEM "https://yukicoder.me/problems/no/1939"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/fps_pow.hpp"

using mint = modint998;

void solve() {
    LL(N, M);
    ++N;

    vector<mint> P(N);
    P[0] = 1;
    For(M) {
        LL(a);
        if (a < N) P[a] += mint(1);
    }
    P = fps_pow_1_sparse<mint>(P, N);
    mint ANS = P[N - 1] / mint(N);
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
