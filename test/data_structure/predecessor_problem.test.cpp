#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#include <library/template.hpp>
#include <library/datastructure/fastset.hpp>

void solve() {
    LL(N, Q);
    FastSet ss(N);
    STR(S);
    For(x, N) {
        if (S[x] == '1') ss.insert(x);
    }

    For(Q) {
        LL(t, k);
        if (t == 0) { ss.insert(k); }
        elif (t == 1) { ss.erase(k); }
        elif (t == 2) { print(ss[k]); }
        elif (t == 3) {
            long long x = ss.next(k);
            if (x == N) x = -1;
            print(x);
        }
        elif (t == 4) { print(ss.prev(k)); }
    }
}

signed main() {
    solve();

    return 0;
}
