#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1566"
#include "library/template.hpp"
#include "library/flow/bipartite_matching.hpp"

void solve() {
    LL(N);
    vector<pair<long long, long long>> AB(N); read(AB);
    long long K = 31;
    for (auto&& [a, b] : AB) {
        --a;
    }
    Graph<bool, 0> G(K + N);
    vector<long long> isin(K);
    For(i, N) {
        auto [a, b] = AB[i];
        For(k, a, b) {
            G.add(k, K + i);
            isin[k] = 1;
        }
    }
    G.build();

    Bipartite_Matching<decltype(G)> X(G);
    long long a = len(X.matching());
    long long b = accumulate(isin.begin(), isin.end(), int(0));
    print((a + b) * 50);
}

signed main() {
    solve();

    return 0;
}
