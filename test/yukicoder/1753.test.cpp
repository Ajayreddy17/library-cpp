#define PROBLEM "https://yukicoder.me/problems/no/1753"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/setfunc/hadamard.hpp"

using mint = modint998;
// using mint = double;

void solve() {
    LL(N);
    vector<mint> P(N + 1);
    {
        vector<long long> A(N + 1); read(A);
        mint den = mint(1) / mint(accumulate(A.begin(), A.end(), (long long)0));
        For(i, N + 1) P[i] = den * mint(A[i]);
    }

    const int K = 10;
    auto calc = [&](int ng) -> vector<mint> {
        vector<mint> X(1 << K);
        For(i, 1, N + 1) {
            if (i == ng) continue;
            X[i] += P[i];
        }
        hadamard<mint>(X);
        // print(X);
        for (auto&& x: X) { x = mint(1) / (mint(1) - x); }
        hadamard<mint>(X);
        return X;
    };

    mint ANS = 0;
    auto A = calc(-1);
    ANS += A[0];
    For(i, 1, N + 1) {
        auto B = calc(i);
        ANS += A[i] - B[i];
    }
    ANS *= P[0] / mint(1 << K);
    ANS = mint(1) - ANS;
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
