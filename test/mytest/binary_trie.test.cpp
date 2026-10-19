#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/datastructure/binary_trie.hpp>
#include <library/random/base.hpp>

void test() {
    const int LOG = 5;
    static Binary_Trie<LOG, false> X;

    For(100) {
        vector<int> A;
        X.reset();
        auto root = X.new_root();
        For(1000) {
            int t = RNG(0, 5);
            if (t == 0) {  // add
                int x = RNG(0, 1 << LOG);
                A.emplace_back(x);
                root = X.add(root, x);
            }
            if (t == 1) {  // get all
                sort(all(A));
                vector<int> B;
                X.enumerate(root, [&](int k, int cnt) -> void { For(cnt) B.emplace_back(k); });
                assert(A == B);
            }
            if (t == 2 && len(A)) {  // erase
                int k = RNG(len(A));
                int x = A[k];
                swap(A[k], A.back());
                A.pop_back();
                X.add(root, x, -1);
            }
            if (t == 3 && len(A)) {  // kth
                int k = RNG(len(A));
                int xor_val = RNG(0, 1 << LOG);
                vector<int> B;
                for (auto&& x : A) B.emplace_back(x ^ xor_val);
                sort(all(B));
                assert(B[k] == int(X.kth(root, k, xor_val)));
            }
            if (t == 4) {  // freq
                int lo = RNG(0, 1 << LOG);
                int hi = RNG(0, 1 << LOG);
                int xor_val = RNG(0, 1 << LOG);
                if (lo > hi) swap(lo, hi);
                ++hi;
                uint32_t cnt = 0;
                for (auto&& x : A) {
                    int y = x ^ xor_val;
                    if (lo <= y && y < hi) ++cnt;
                }
                assert(cnt == X.count(root, lo, hi, xor_val));
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
