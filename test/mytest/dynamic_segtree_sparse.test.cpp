#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/monoid/monoid_min.hpp>
#include <library/datastructure/segment_tree/dynamic_segtree_sparse.hpp>
#include <library/random/base.hpp>

void test() {
    using Mono = Monoid_Min<int>;
    int id = Mono::unit();

    For(100) {
        int N = RNG(1, 100);

        vector<int> A(N, id);
        Dynamic_SegTree_Sparse<Mono, false> X(500000, 0, N);
        using np = typename decltype(X)::np;
        np root = X.new_root();

        int Q = RNG(1, 1000);

        For(Q) {
            int t = RNG(0, 4);
            int L = RNG(0, N);
            int R = RNG(0, N);
            if (L > R) swap(L, R);
            ++R;
            if (t == 0) {
                int i = RNG(0, N);
                int x = RNG(1, 100);
                root = X.set(root, i, x);
                A[i] = x;
            }
            if (t == 1) {
                int i = RNG(0, N);
                int x = RNG(1, 100);
                root = X.multiply(root, i, x);
                chmin(A[i], x);
            }
            if (t == 2) {
                vector<int> B = {A.begin() + L, A.begin() + R};
                assert(X.prod(root, L, R) == *min_element(B.begin(), B.end()));
            }
            if (t == 3) {
                // max_right
                int LIM = RNG(1, 100);
                auto check = [&](auto e) -> bool { return e >= LIM; };
                int naive = [&]() -> int {
                    long long mi = id;
                    For(i, L, N) {
                        chmin(mi, A[i]);
                        if (mi < LIM) return i;
                    }
                    return N;
                }();
                assert(naive == X.max_right(root, L, check));
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
