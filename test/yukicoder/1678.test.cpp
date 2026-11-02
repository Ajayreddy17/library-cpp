#define PROBLEM "https://yukicoder.me/problems/no/1678"
#include "library/template.hpp"
#include "library/flow/mincostflow.hpp"

void solve() {
    LL(N, K);
    vector<long long> A(N);
    vector<vector<int>> B(N);
    For(i, N) {
        LL(a, m);
        A[i] = a;
        For(m) {
            LL(b);
            B[i].emplace_back(--b);
        }
    }

    int source = 0;
    int sink = N + 1;
    auto idx = [&](int v) -> int { return 1 + v; };

    Min_Cost_Flow<int, long long, 1> G(N + 2, source, sink);
    For(i, N + 1) G.add(i, i + 1, K, 0);
    For(to, N) {
        for (auto&& frm : B[to]) {
            long long cost = A[frm] - A[to];
            G.add(idx(frm), idx(to), 1, cost);
        }
    }
    auto XY = G.slope();
    long long ANS = -XY.back().second;
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
