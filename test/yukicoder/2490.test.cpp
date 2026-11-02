#define PROBLEM "https://yukicoder.me/problems/no/2490"
#include "library/template.hpp"
#include "library/datastructure/bit_array.hpp"

void solve() {
    INT(N);
    vector<int> A(2 * N); read(A);
    For(i, 2 * N) A.emplace_back(A[i]);
    using BS = Bit_Array;
    BS ng(7 * N);
    BS other(4 * N);
    vector<vector<int>> pos(2 * N + 1);
    For(i, len(A)) {
        if (A[i] == -1) continue;
        pos[A[i]].emplace_back(i);
        other[i] = 1;
    }

    For(x, 1, 2 * N + 1) {
        auto& I = pos[x];
        for (auto& i: I) { other[i] = 0; }
        for (auto& i: I) {
            if (i >= 3 * N) break;
            BS x = other.slice(i + 1, i + N + 1);
            ng.or_to_range(2 * i + 1, 2 * i + N + 1, x);
        }
        for (auto& i: I) { other[i] = 1; }
    }

    int s = N - 1, t = 5 * N - 1;
    For(i, s, t) if (!ng[i]) return print(Yes[1]);
    print(Yes[!(1)]);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
