#define PROBLEM  "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B"
#include "library/template.hpp"
#include "library/flow/mincostflow.hpp"

void solve() {
    LL(N, M, F);
    Min_Cost_Flow<int, long long, 0> G(N, 0, N - 1);
    For(M) {
        LL(a, b, c, d);
        G.add(a, b, c, d);
    }

    auto [f, x] = G.flow(F);
    if (f < F)
        print(-1);
    else
        print(x);
}

signed main() {
    solve();
    return 0;
}
