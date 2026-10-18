#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"
#include <library/template.hpp>
#include <library/monoid/monoid_add.hpp>
#include <library/datastructure/segment_tree/dynamic_segtree_sparse.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    Dynamic_SegTree_Sparse<Monoid_Add<int>, true> seg(0, N);
    auto I = argsort(A);

    vector<int> roots;
    roots.emplace_back(0);
    For(k, N) { roots.emplace_back(seg.set(roots.back(), I[k], 1)); }
    For(Q) {
        LL(L, R, k);
        auto check = [&](long long t) -> bool { return seg.prod(roots[t], L, R) <= k; };
        long long t = binary_search(check, 0, N);
        print(A[I[t]]);
    }
}

signed main() {
    solve();

    return 0;
}
