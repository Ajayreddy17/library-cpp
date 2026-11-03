#ifndef AJAY_OPTIMAL_PRODUCT_ON_TREE
#define AJAY_OPTIMAL_PRODUCT_ON_TREE

#include "library/graph/tree.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree/optimal_product_on_tree.hpp

// each vertex holds an element of a totally ordered monoid (x<=y -> axb<=ayb)
// maximise the product taken in a topological order
// https://atcoder.jp/contests/agc023/tasks/agc023_f
// https://qoj.ac/problem/6561
// not abstract enough: maybe x<=y iff xy<=yx is needed?
template <typename TREE, typename Monoid, bool MINIMIZE, typename F>
pair<vector<int>, typename Monoid::value_type> optimal_product_on_tree(TREE& tree, vector<typename Monoid::value_type> A, F compare) {
    const int N = tree.N;
    const int root = tree.V[0];
    using X = typename Monoid::value_type;

    vector<int> head(N), tail(N), nxt(N);
    For(v, N) head[v] = v, tail[v] = v, nxt[v] = -1;

    UnionFind uf(N);

    int size = 1;
    while (size < N) size *= 2;
    vector<int> seg(2 * size, -1);

    // a priority queue got confusing with dynamic data, so use a segment tree
    auto upd = [&](int i) -> void {
        int a = seg[2 * i + 0], b = seg[2 * i + 1];
        if (a == -1) seg[i] = b;
        elif (b == -1) seg[i] = a;
        else seg[i] = (compare(A[a], A[b]) ? a : b);
    };
    // tried 'if seg[i]==x: return', but the data changes, so that is wrong
    auto set = [&](int i, int x) -> void {
        i += size;
        seg[i] = x;
        while (i > 1) { i /= 2, upd(i); }
    };
    For(i, N) if (i != root) seg[size + i] = i;
    Frr(i, 1, size) upd(i);

    For(N - 1) {
        int v = seg[1];
        int a = head[uf[v]], b = tail[uf[v]];
        int p = uf[tree.parent[a]];
        int c = head[p], d = tail[p];
        X pv = Monoid::op(A[p], A[v]);
        uf.merge(p, v);
        int w = uf[v];
        A[w] = pv, head[w] = c, tail[w] = b, nxt[d] = a;
        int av = -1, ap = -1;
        if (v == w && uf[v] != uf[root]) av = v;
        if (p == w && uf[p] != uf[root]) ap = p;
        set(v, av), set(p, ap);
    }
    vector<int> I = {root};
    while (nxt[I.back()] != -1) I.emplace_back(nxt[I.back()]);
    return {I, A[uf[root]]};
}
} // namespace mitsuha
#endif // AJAY_OPTIMAL_PRODUCT_ON_TREE
