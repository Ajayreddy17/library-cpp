#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"
#include <library/template.hpp>
#include <library/monoid/monoid_add.hpp>
#include <library/datastructure/segment_tree/dynamic_segtree.hpp>
#include <library/util/binary_serch.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    Dynamic_SegTree<Monoid_Add<int>, true> seg(1000000, 0, N);
    using np = decltype(seg)::np;
    auto I = argsort(A);

    vector<np> roots;
    roots.emplace_back(seg.new_node(0, N));
    For(k, N) { roots.emplace_back(seg.set(roots.back(), I[k], 1)); }
    For(Q) {
        LL(L, R, k);
        auto check = [&](long long t) -> bool { return seg.prod(roots[t], L, R) <= k; };
        long long t = binary_search<LastTrue>(0LL, (long long)N, check);
        print(A[I[t]]);
    }
}

signed main() {
    solve();

    return 0;
}
