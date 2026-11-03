#define PROBLEM "https://yukicoder.me/problems/no/2320"
#include "library/template.hpp"
#include "library/flow/binary_optimization.hpp"

void solve() {
    LL(N, S, T);

    vector<int> state(N, -1);
    For(S) {
        INT(x);
        --x;
        state[x] = 0;
    }
    For(T) {
        INT(x);
        --x;
        state[x] = 1;
    }

    Binary_Optimization<int128, false> X(N);
    For(i, N) {
        if (state[i] == 0) X.add_1(i, 0, -linf);
        if (state[i] == 1) X.add_1(i, -linf, 0);
    }
    For(i, N) For(j, N) {
        INT(x);
        if (i >= j) continue;
        X.add_2(i, j, x, 0, 0, x);
    }

    auto [val, x] = X.calc();
    print(val);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
