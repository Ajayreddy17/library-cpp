#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"

#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree_2d.hpp>

void solve() {
    LL(N, Q);
    vector<uint32_t> X(N), Y(N);
    vector<uint64_t> W(N);
    For(i, N) read(X[i], Y[i], W[i]);
    using QQ = tuple<uint32_t, uint32_t, uint32_t, uint32_t>;
    vector<QQ> query(Q);
    For(q, Q) {
        LL(t);
        if (t == 0) {
            U32(x, y, w);
            X.emplace_back(x);
            Y.emplace_back(y);
            W.emplace_back(0);
            query[q] = make_tuple(-1, x, y, w);
        } else {
            U32(a, b, c, d);
            query[q] = make_tuple(a, c, b, d);
        }
    }

    FenwickTree_2D<Monoid_Add<uint64_t>, uint32_t, false> bit(X, Y, W);
    For(q, Q) {
        auto [a, b, c, d] = query[q];
        if (a == uint32_t(-1)) {
            bit.add(b, c, d);
        } else {
            print(bit.prod(a, b, c, d));
        }
    }
}

signed main() {
    solve();

    return 0;
}
