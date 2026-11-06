#define PROBLEM "https://yukicoder.me/problems/no/1625"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_max.hpp"

void solve() {
    LL(N, Q);
    // triangle -> point (min x, max x) with weight 2 * area
    vector<tuple<long long, long long, long long>> query;
    auto tri = [&]() {
        LL(a, b, c, d, e, f);
        long long x = min({a, c, e}), y = max({a, c, e});
        long long area = abs((c - a) * (f - b) - (e - a) * (d - b));
        query.emplace_back(x, y, area);
    };
    For(N) tri();
    For(Q) {
        LL(t);
        if (t == 1) tri();
        if (t == 2) {
            LL(l, r);
            query.emplace_back(-1, l, r + 1);
        }
    }
    using Mono = Monoid_Max<long long>;
    Wavelet_Matrix_2D_Range<int, true, true, SegTree<Mono>> WM(N + Q, [&](int i) -> tuple<int, int, long long> {
        auto [a, b, c] = query[i];
        if (i < N) return {a, b, c};
        return {a, b, Mono::unit()};
    });
    For(q, N, N + Q) {
        auto [a, b, c] = query[q];
        if (a != -1) {
            WM.multiply(q, c);
        } else {
            long long ANS = WM.prod(b, c, b, c);
            print(ANS == Mono::unit() ? -1 : ANS);
        }
    }
}

signed main() {
    solve();
    return 0;
}
