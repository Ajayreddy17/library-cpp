#define PROBLEM  "https://judge.yosupo.jp/problem/vertex_get_range_contour_add_on_tree"
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
    FenwickTree<Monoid_Add<long long>> bit(len(CQ) + 1);

    auto apply = [&](int v, int l, int r, int x) -> void {
        for (auto& [a, b]: CQ.get_contour_range(v, l, r)) {
            bit.add(a, x), bit.add(b, -x);
        }
        if (l <= 0 && 0 < r) A[v] += x;
    };

    auto get = [&](int v) -> long long {
        long long ans = A[v];
        for (auto& i: CQ.get_indices(v)) { ans += bit.sum(i + 1); }
        return ans;
    };

    For(Q) {
        INT(t);
        if (t == 0) {
            INT(v, l, r, x);
            apply(v, l, r, x);
        }
        if (t == 1) {
            INT(v);
            print(get(v));
        }
    }
}

signed main() {
    solve();
    return 0;
}
