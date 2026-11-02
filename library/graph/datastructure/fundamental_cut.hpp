#ifndef AJAY_FUNDAMENTAL_CUT
#define AJAY_FUNDAMENTAL_CUT

#include "library/graph/tree.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_maxidx.hpp"
#include "library/monoid/monoid_minidx.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/fundamental_cut.hpp

// given a tree and M vertex pairs
// set_on, set_off: switch each of the M pairs on or off
// query(v): one index of a pair separated by edge v - par[v]
// https://codeforces.com/contest/1889/problem/E
template <typename TREE>
struct Fundamental_Cut {
    TREE& tree;

    int N, M;
    vector<int> A, B, on;
    vector<int> IA, IB, rk_A, rk_B, FA, FB;
    SegTree<Monoid_Max_Idx<int>> seg_AB;
    SegTree<Monoid_Min_Idx<int>> seg_BA;

    // f(i) = a, b, bool(on,off)
    template <typename F>
    Fundamental_Cut(TREE& tree, int M, F f) : tree(tree), N(tree.N), M(M) {
        For(i, M) {
            auto [a, b, t] = f(i);
            a = tree.LID[a], b = tree.LID[b];
            if (a > b) swap(a, b);
            A.emplace_back(a), B.emplace_back(b), on.emplace_back(t);
        }
        IA = argsort(A), IB = argsort(B);
        rk_A = argsort(IA), rk_B = argsort(IB);
        seg_AB.build(M, [&](int i) -> pair<int, int> {
            if (!on[i]) return {-infty<int>, -1};
            return {B[IA[i]], IA[i]};
        });
        seg_BA.build(M, [&](int i) -> pair<int, int> {
            if (!on[i]) return {infty<int>, -1};
            return {A[IB[i]], IB[i]};
        });
        FA.resize(N + 1), FB.resize(N + 1);
        for (auto x : A) FA[1 + x]++;
        for (auto x : B) FB[1 + x]++;
        For(i, N) FA[i + 1] += FA[i], FB[i + 1] += FB[i];
    }

    void set_on(int i) {
        if (on[i]) return;
        on[i] = 1;
        seg_AB.set(rk_A[i], {B[i], i}), seg_BA.set(rk_B[i], {A[i], i});
    }

    void set_off(int i) {
        if (!on[i]) return;
        on[i] = 0;
        seg_AB.set(rk_A[i], {-infty<int>, -1}),
                seg_BA.set(rk_B[i], {infty<int>, -1});
    }

    int query(int v) {
        int L = tree.LID[v], R = tree.RID[v];
        int i = seg_AB.prod(FA[L], FA[R]).second;
        if (i != -1 && R <= B[i]) return i;
        i = seg_BA.prod(FB[L], FB[R]).second;
        if (i != -1 && A[i] < L) return i;
        return -1;
    }
};
} // namespace mitsuha
#endif // AJAY_FUNDAMENTAL_CUT
