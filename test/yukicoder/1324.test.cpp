#define PROBLEM "https://yukicoder.me/problems/no/1324"
#include "library/template.hpp"
#include "library/flow/mincostflow.hpp"

void solve() {
    LL(N, K);
    vector<long long> A(N); read(A);
    vector<long long> B(N); read(B);
    auto left = [&](int i) -> int { return 1 + i; };
    auto right = [&](int i) -> int { return 1 + N + i; };
    int source = 0;
    int sink = 1 + N + N;
    Min_Cost_Flow<int, int, true> G(N + N + 2, source, sink);

    vector<vector<long long>> P(N, vector<long long>(N)); read(P);
    For(i, N) G.add(source, left(i), A[i], 0);
    For(i, N) G.add(right(i), sink, B[i], 0);

    long long base = 0;
    For(i, N) For(j, N) {
        long long y = P[i][j];
        base += y * y;
        For(k, min(A[i], B[j]) + 1) {
            long long a = (y - k) * (y - k);
            long long b = (y - k - 1) * (y - k - 1);
            G.add(left(i), right(j), 1, b - a);
        }
    }
    auto [flow, cost] = G.flow();
    print(base + cost);
}

signed main() {
    solve();
    return 0;
}
