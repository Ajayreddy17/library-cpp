#define PROBLEM "https://yukicoder.me/problems/no/2288"
#include "library/template.hpp"
#include "library/graph/stable_matching.hpp"

void solve() {
    LL(N, X, Y);
    N += 2;
    vector<pair<long long, long long>> col1(N, {-1, -1});
    vector<pair<long long, long long>> col2(N, {-1, -1});
    vector<vector<int>> P, Q;
    For(i, X) {
        INT(n);
        ++n;
        vector<int> A(n); read(A);
        For(j, 1, n - 1) col1[A[j]] = {i, j};
        P.emplace_back(A);
    }
    For(i, Y) {
        INT(n);
        ++n;
        vector<int> A(n); read(A);
        For(j, 1, n - 1) col2[A[j]] = {i, j};
        Q.emplace_back(A);
    }
    Stable_Matching S(Y, X);
    For(v, 1, N - 1) {
        auto [i, ni] = col1[v];
        auto [j, nj] = col2[v];
        if (i == -1 || j == -1) continue;
        S.add(j, i, nj, -ni);
    }

    S.calc();

    long long ANS = 0;
    For(i, X) {
        long long x = -S.val_2[i];
        ANS += min(x, len(P[i]) - 1);
    }
    print(ANS);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
