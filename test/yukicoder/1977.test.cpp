#define PROBLEM "https://yukicoder.me/problems/no/1977"
#include "library/template.hpp"
#include "library/datastructure/doubling.hpp"
#include "library/monoid/monoid_add.hpp"

using GRP = Monoid_Add<long long>;

void solve() {
    LL(N, M, L);
    vector<long long> A(N); read(A);
    Doubling<GRP, 50> X(N);
    For(i, N) {
        int j = (i + L) % N;
        X.add(i, j, A[j]);
    }
    X.build();

    long long ANS = -linf;

    For(i, N) {
        long long s0 = i;
        long long s1 = L / N * N + i;
        while (s0 >= L) s0 -= N;
        while (s1 >= L) s1 -= N;
        long long a0 = (N * M - 1 - s0) / L;
        long long a1 = (N * M - 1 - s1) / L;
        if (s0 >= 0) chmax(ANS, X.calc(i, a0).second + A[i]);
        if (s1 >= 0) chmax(ANS, X.calc(i, a1).second + A[i]);
    }
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
