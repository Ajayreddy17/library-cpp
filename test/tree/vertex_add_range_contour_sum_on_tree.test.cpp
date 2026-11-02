#define PROBLEM  "https://judge.yosupo.jp/problem/vertex_add_range_contour_sum_on_tree"
#include "library/template.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree.hpp"
#include "library/graph/datastructure/contour_query_range.hpp"
#include "library/monoid/monoid_add.hpp"

void solve() {
    INT(N, Q);
    vector<long long> A(N); read(A);
    Graph<int, 0> G(N);
    G.read_tree(0, 0);
    Contour_Query_Range<decltype(G), false> CQ(G);
    vector<long long> dat_raw(len(CQ));
    For(v, N) {
        for (auto& p: CQ.get_indices(v)) { dat_raw[p] += A[v]; }
    }
    FenwickTree<Monoid_Add<long long>> bit(dat_raw);

    For(Q) {
        INT(t);
        if (t == 0) {
            INT(v, x);
            A[v] += x;
            for (auto& p: CQ.get_indices(v)) bit.add(p, x);
        }
        if (t == 1) {
            INT(v, l, r);
            long long ans = 0;
            if (l <= 0 && 0 < r) ans += A[v];
            for (auto& [a, b]: CQ.get_contour_range(v, l, r)) {
                ans += bit.sum(a, b);
            }
            print(ans);
        }
    }
}

signed main() {
    solve();
    return 0;
}
