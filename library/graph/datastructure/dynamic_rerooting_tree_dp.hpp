#ifndef AJAY_DYNAMIC_REROOTING_TREE_DP
#define AJAY_DYNAMIC_REROOTING_TREE_DP

#include "library/graph/datastructure/static_toptree.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/dynamic_rerooting_tree_dp.hpp

/*
struct Data {
    // always define type, s, t (you will need them when debugging anyway). s is the root.
    // type==0: s is virtual. type==1: t is virtual.
    int type, s, t;
};

struct TREE_DP {
    using value_type = Data;
    using X = value_type;

    static X rake(const X& L, const X& R) {
        assert(L.type == 0 && R.type == 0 && L.s == R.s);
        X ANS = {0, L.s, L.t};
    }
    static X rake2(const X& L, const X& R) {
        assert(L.type == 1 && R.type == 0 && L.s == R.s);
        X ANS = {1, L.s, L.t};
    }
    static X compress(const X& L, const X& R) {
        assert(L.type == 0 && R.type == 0 && L.t == R.s);
        X ANS = {0, L.s, R.t};
    }
    static X compress2(const X& L, const X& R) {
        assert(L.type == 1 && R.type == 1 && L.t == R.s);
        X ANS = {1, L.s, R.t};
    }
    static X compress3(const X& L, const X& R) {
        assert(L.type == 1 && R.type == 0 && L.t == R.s);
        X ANS = {1, L.s, L.t};
    }
};
*/

template <typename TREE, typename TREE_DP>
struct Dynamic_Rerooting_Tree_DP {
    using X = typename TREE_DP::value_type;
    Static_TopTree<TREE> STT;
    vector<pair<X, X>> dp;

    template <typename F>
    Dynamic_Rerooting_Tree_DP(TREE& tree, F f) : STT(tree) {
        assert(tree.V[0] == 0);  // shortcut
        int N = tree.N;
        dp.resize(2 * N - 1);
        For(v, N) {
            dp[v] = f(v);
            assert(dp[v].first.type == 0 && dp[v].second.type == 1);
            assert(dp[v].first.s == STT.tree.parent[v] && dp[v].first.t == v);
            assert(dp[v].second.t == STT.tree.parent[v] && dp[v].second.s == v);
        }
        For(i, N, 2 * N - 1) update(i);
    }

    // edge between v and its parent: parent virtual / child virtual
    void set(int v, pair<X, X> x) {
        dp[v] = x;
        assert(dp[v].first.type == 0 && dp[v].second.type == 1);
        assert(dp[v].first.s == STT.tree.parent[v] && dp[v].first.t == v);
        assert(dp[v].second.t == STT.tree.parent[v] && dp[v].second.s == v);
        for (int i = STT.par[v]; i != -1; i = STT.par[i]) {
            update(i);
        }
    }

    X prod_all(int v) {
        int i = v;
        X a = dp[i].second, b, c;
        b.type = c.type = -1;
        while (1) {
            int p = STT.par[i];
            if (p == -1) break;
            int l = STT.lch[p], r = STT.rch[p];
            if (STT.is_compress[p]) {
                if (l == i) {
                    b = (b.type == -1 ? dp[r].first : TREE_DP::compress(b, dp[r].first));
                } else {
                    a = (a.type == -1 ? dp[l].second : TREE_DP::compress2(a, dp[l].second));
                }
            } else {
                if (STT.lch[p] == i) {
                    if (a.type == -1) {
                        b = (b.type == -1 ? dp[r].first : TREE_DP::rake(b, dp[r].first));
                    } else {
                        a = TREE_DP::compress3(a, dp[r].first);
                    }
                } else {
                    if (a.type == -1) {
                        c = TREE_DP::compress3(c, b);
                        a.type = -1;
                        b = dp[l].first;
                    } else {
                        a = (b.type == -1 ? a : TREE_DP::rake2(a, b));
                        c = (c.type == -1 ? a : TREE_DP::compress2(c, a));
                        a.type = -1;
                        b = dp[l].first;
                    }
                }
            }
            i = p;
        }
        a = (b.type == -1 ? a : TREE_DP::rake2(a, b));
        return (c.type == -1 ? a : TREE_DP::compress2(c, a));
    }

  private:
    inline void update(int i) {
        auto& [L1, L2] = dp[STT.lch[i]];
        auto& [R1, R2] = dp[STT.rch[i]];
        if (STT.is_compress[i]) {
            dp[i] = {TREE_DP::compress(L1, R1), TREE_DP::compress2(R2, L2)};
        } else {
            dp[i] = {TREE_DP::rake(L1, R1), TREE_DP::compress3(L2, R1)};
        }
    }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_REROOTING_TREE_DP
