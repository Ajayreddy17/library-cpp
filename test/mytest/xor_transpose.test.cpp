#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/random/base.hpp>
#include <library/linear_algebra/xor/transpose.hpp>

template <typename UINT>
vector<UINT> naive(int n, int m, vector<UINT> mat) {
    vector<UINT> after(m);
    For(i, n) For(j, m) if (mat[i] >> j & 1) after[j] |= UINT(1) << i;
    return after;
}

void test_32() {
    For(100) {
        For(n, 33) For(m, 33) {
            vector<uint32_t> A(n);
            For(i, n) For(j, m) {
                uint32_t x = RNG(2);
                A[i] |= x << j;
            }
            assert(transpose<uint32_t>(n, m, A) == naive<uint32_t>(n, m, A));
        }
    }
}

void test_64() {
    For(100) {
        For(n, 65) For(m, 65) {
            vector<uint64_t> A(n);
            For(i, n) For(j, m) {
                uint64_t x = RNG(2);
                A[i] |= x << j;
            }
            assert(transpose<uint64_t>(n, m, A) == naive<uint64_t>(n, m, A));
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test_32();
    solve();
    return 0;
}