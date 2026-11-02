#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/datastructure/bit_array.hpp"

void test() {
    For(N, 2000) {
        int Q = 10 * N;
        vector<int> A(N);
        Bit_Array B(N);
        For(Q) {
            int t = RNG(0, 4);
            int i = RNG(0, N);
            if (t == 0) {
                A[i] = 0;
                B[i] = 0;
            }
            if (t == 1) {
                A[i] = 1;
                B[i] = 1;
            }
            if (t == 2) {
                int p = i;
                while (p < N && A[p] == 0) ++p;
                assert(B.next(i) == p);
            }
            if (t == 3) {
                int p = i;
                while (p >= 0 && A[p] == 0) --p;
                assert(B.prev(i) == p);
            }
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test();
    solve();
    return 0;
}
