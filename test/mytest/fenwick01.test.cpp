#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/random/base.hpp>
#include <library/datastructure/fenwick_tree/fenwicktree_01.hpp>

void test() {
    For(N, 1, 1000) {
        For(Q, 1, 100) {
            vector<int> A(N);
            FenwickTree_01 bit(N);
            For(Q) {
                int t = RNG(0, 2);
                if (t == 0) {
                    int k = RNG(0, N);
                    if (A[k] == 0)
                        bit.add(k), A[k] = 1;
                    else
                        bit.remove(k), A[k] = 0;
                }
                if (t == 1) {
                    vector<int> I;
                    For(i, N) if (A[i]) I.emplace_back(i);
                    if (I.empty()) continue;
                    int k = RNG(0, len(I));
                    assert(bit.kth(k) == I[k]);
                }
            }
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
}

signed main() {
    test();
    solve();
    return 0;
}