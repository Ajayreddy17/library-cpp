#define PROBLEM "https://yukicoder.me/problems/no/1541"
#include "library/template.hpp"
#include "library/flow/binary_optimization.hpp"

void solve() {
    LL(N, M);
    Binary_Optimization<long long, false> X(N);

    For(i, N) {
        LL(k, C);
        X.add_1(i, 0, M - C);
        vector<long long> A(k); read(A);
        vector<long long> B(k); read(B);
        for (auto&& x: A) --x;
        For(j, k) { X.add_2(i, A[j], 0, 0, 0, B[j]); }
    }
    auto [a, x] = X.calc();
    print(a);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
