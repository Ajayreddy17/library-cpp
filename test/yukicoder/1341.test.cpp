#define PROBLEM "https://yukicoder.me/problems/no/1341"
#include "library/template.hpp"
#include "library/flow/mincostflow.hpp"

void solve() {
    LL(N, M);
    vector<long long> A(N), X(N), C(N);
    For(i, N) {
        LL(a, b, c);
        if (a > c) swap(a, c);
        A[i] = a, C[i] = c;
        X[i] = b;
    }
    sort(X.begin(), X.end());

    int source = 0;
    auto idx_ac = [&](int i) -> int { return 1 + i; };
    auto idx_a = [&](int i) -> int { return 1 + N + i; };
    auto idx_c = [&](int i) -> int { return 1 + N + N + i; };
    auto idx_x = [&](int i) -> int { return 1 + 3 * N + (N - 1 - i); };
    auto idx_y = [&](int i) -> int { return 1 + 4 * N + i; };
    auto idx_xy = [&](int i) -> int { return 1 + 5 * N + i; };
    int sink = 1 + 6 * N;
    Min_Cost_Flow<int, long long, true> G(sink + 1, source, sink);

    // x: used below a
    // y: used above c
    For(i, N) G.add(source, idx_ac(i), 1, 0);
    For(i, N) { G.add(idx_ac(i), idx_a(i), 1, -C[i]); }
    For(i, N) { G.add(idx_ac(i), idx_c(i), 1, 0); }
    For(i, N) { G.add(idx_x(i), idx_xy(i), 1, 0); }
    For(i, N) { G.add(idx_y(i), idx_xy(i), 1, -X[i]); }
    For(i, N) { G.add(idx_xy(i), sink, 1, 0); }
    For(i, N - 1) { G.add(idx_x(i + 1), idx_x(i), N, 0); }
    For(i, N - 1) { G.add(idx_y(i), idx_y(i + 1), N, 0); }

    For(i, N) {
        // largest value < A[i]
        int k = int(lower_bound(X.begin(), X.end(), A[i]) - X.begin()) - 1;
        if (k != -1) G.add(idx_a(i), idx_x(k), 1, 0);
    }
    For(i, N) {
        // smallest value > C[i]
        int k = upper_bound(X.begin(), X.end(), C[i]) - X.begin();
        if (k != N) G.add(idx_c(i), idx_y(k), 1, 0);
    }

    auto [flow, cost] = G.flow();
    print(YES[flow == N]);
    if (flow == N) {
        cost = -cost;
        print(cost >= M ? "KADOMATSU!" : "NO");
    }
}

signed main() {
    solve();
    return 0;
}
