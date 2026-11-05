#define PROBLEM "https://yukicoder.me/problems/no/1242"
#include "library/template.hpp"
#include "library/graph/cycle/functional.hpp"

void solve() {
    LL(N, K);
    VT(long long, A, K);

    FunctionalGraph<> FG(64);
    For(s, 64) {
        int t = (2 * s) & 63;
        bool ok = 1;
        if (!(s & 1) && !(s & 32)) ok = 0;
        if (!(s & 2) && !(s & 16)) ok = 0;
        if (!(s & 4) && !(s & 8)) ok = 0;
        if (ok) t |= 1;
        FG.add(s, t);
    }
    FG.build();
    long long x = N;
    long long s = 63;
    Frr(k, K) {
        long long y = A[k];
        s = FG.jump(s, x - y).first;
        s &= 62;
        x = y;
    }
    s = FG.jump(s, x - 1).first;
    print(s & 1 ? "Yes" : "No");
}

signed main() {
    solve();
    return 0;
}
