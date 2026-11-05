#define PROBLEM "https://yukicoder.me/problems/no/1211"
#include "library/template.hpp"
#include "library/graph/cycle/functional.hpp"
#include "library/util/binary_serch.hpp"

void solve() {
    LL(N, K);
    VT(int, A, N);
    For(i, N) A.emplace_back(A[i]);
    vector<long long> Ac(N + N + 1);
    For(i, N + N) Ac[i + 1] = Ac[i] + A[i];

    auto check = [&](long long S) -> bool {
        // every piece can be made >= S
        if (S <= 0) return 1;
        FunctionalGraph<> FG(N + N + 1);
        int j = 0;
        For(i, N + N + 1) {
            while (j < N + N && Ac[j] < Ac[i] + S) ++j;
            FG.add(i, j);
        }
        FG.build();
        auto TO = FG.jump_all(K);
        For(i, N) if (TO[i] <= i + N) return true;
        return false;
    };

    long long ANS = binary_search<LastTrue>(0LL, Ac[N] / K, check);
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
