#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/acted_monoid/am_summax_assign.hpp>
#include <library/datastructure/segment_tree/dynamic_lazy_segtree.hpp>
#include <library/random/base.hpp>

void test() {
    using AM = ActedMonoid_SumMax_Assign<int, -1>;
    using P = typename AM::X;

    For(100) {
        int N = RNG(1, 1000);

        vector<vector<int>> AA;
        AA.emplace_back(vector<int>(N, 10));
        Dynamic_Lazy_SegTree<AM, true> X(
                500000, 0, N, [](long long l, long long r) -> P { return {10 * (r - l), 10}; });
        using np = typename decltype(X)::np;

        int Q = RNG(1, 1000);
        vector<np> roots;
        roots.emplace_back(X.new_node(0, N));

        For(Q) {
            int time = RNG(0, len(roots));
            vector<int> A = AA[time];
            np root = roots[time];
            int L = RNG(0, N);
            int R = RNG(0, N);
            if (L > R) swap(L, R);
            ++R;

            int t = RNG(0, 4);
            if (t == 0) {
                int i = RNG(0, N);
                int x = RNG(1, 100);
                root = X.set(root, i, {x, x});
                A[i] = x;
            }
            if (t == 1) {
                vector<int> B = {A.begin() + L, A.begin() + R};
                assert(X.prod(root, L, R).first == std::accumulate(B.begin(), B.end(), int{}));
                assert(X.prod(root, L, R).second == *max_element(B.begin(), B.end()));
            }
            if (t == 2) {
                int x = RNG(1, 100);
                For(i, L, R) A[i] = x;
                root = X.apply(root, L, R, x);
            }
            if (t == 3) {
                // max_right
                int LIM = R;
                auto check = [&](auto e) -> bool { return e.second <= LIM; };
                int naive = [&]() -> int {
                    long long mx = 0;
                    For(i, L, N) {
                        chmax(mx, A[i]);
                        if (mx > LIM) return i;
                    }
                    return N;
                }();

                assert(naive == X.max_right(root, L, check));
            }

            AA.emplace_back(A);
            roots.emplace_back(root);
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
