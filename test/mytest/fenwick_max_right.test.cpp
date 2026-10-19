#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree.hpp>
#include <library/random/base.hpp>

void test(int N) {
    FenwickTree<Monoid_Add<int>> bit(N);

    For(1 << 15) {
        bit.set(RNG(0, N), RNG(0, 10));
        long long s = RNG(0, N + 1);
        long long sm = bit.sum(s, N);
        long long K = RNG(0, 2 * sm + 1);
        {
            long long god = bit.max_right(s, [&](long long x) -> bool { return x <= K; });
            long long ans = s;
            while (ans < N && bit.prod(s, ans + 1) <= K) ans++;
            assert(god == ans);
        }
    }

    For(1 << 15) {
        bit.set(RNG(0, N), RNG(0, 10));
        long long s = RNG(0, N + 1);
        long long sm = bit.sum(s, N);
        long long K = s + RNG(0, 2 * sm + 1);
        {
            long long god = bit.max_right_with_index(s, [&](long long x, long long i) -> bool { return i + x <= K; });
            long long ans = s;
            while (ans < N && (ans + 1) + bit.prod(s, ans + 1) <= K) ans++;
            assert(god == ans);
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    For(N, 1, 100) test(N);
    solve();
}
