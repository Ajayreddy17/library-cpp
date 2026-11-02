#define PROBLEM "https://yukicoder.me/problems/no/1038"
#include "library/template.hpp"
#include "library/graph/datastructure/contour_query_range.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

void solve() {
    LL(N, Q);
    Graph<int, 0> G(N);
    G.read_tree();

    Contour_Query_Range<decltype(G), false> X(G);
    FenwickTree<Monoid_Add<long long>> bit(len(X) + 1);
    vector<long long> A(N);

    For(Q) {
        INT(v, d, x);
        --v;
        long long ans = A[v];
        for (auto& i: X.get_indices(v)) { ans += bit.sum(1 + i); }
        print(ans);

        for (auto& [a, b]: X.get_contour_range(v, 0, 1 + d)) {
            bit.add(a, x), bit.add(b, -x);
        }
        A[v] += x;
    }
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
