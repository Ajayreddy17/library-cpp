#ifndef AJAY_REROOTING
#define AJAY_REROOTING

#include "library/graph/tree.hpp"

namespace mitsuha{
template <typename TREE, typename Data>
struct Rerooting_dp {
    static_assert(!TREE::Graph_type::is_directed);
    TREE& tree;
    vector<Data> dp_1; // For edge pv, subtree v
    vector<Data> dp_2; // For edge pv, subtree p
    vector<Data> dp;   // full tree

    template <typename F1, typename F2, typename F3>
    Rerooting_dp(TREE& tree, F1 f_ee, F2 f_ev, F3 f_ve, const Data unit) : tree(tree) {
        build(f_ee, f_ev, f_ve, unit);
    }

    Data operator[](int v) { return dp[v]; }

    // Subtree v when root is the root
    Data get(int v, int root) {
        if (root == v) return dp[v];
        if (!tree.in_subtree(root, v)) { return dp_1[v]; }
        int w = tree.jump(v, root, 1);
        return dp_2[w];
    }

    template <typename F1, typename F2, typename F3>
    void build(F1 f_ee, F2 f_ev, F3 f_ve, const Data unit) {
        int N = tree.N;
        // dp1: subtree
        dp_1.assign(N, unit);
        for(int i = N - 1; i >= 0; --i) {
            int v = tree.V[i];
            for (auto&& e: tree.G[v]) {
                if (e.to == tree.parent[v]) continue;
                dp_1[v] = f_ee(dp_1[v], f_ve(dp_1[e.to], e));
            }
            dp_1[v] = f_ev(dp_1[v], v);
        }

        // dp2[v]: subtree of p, rooted at v
        dp_2.assign(N, unit);
        // dp[v]: fulltree, rooted at v
        dp.assign(N, unit);
        for(int i = 0; i < N; ++i) {
            int p = tree.V[i];
            vector<int> ch;
            vector<Data> ch_data;
            Data x = unit;
            for (auto&& e: tree.G[p]) {
                if (e.to == tree.parent[p]) {
                    x = f_ve(dp_2[p], e);
                } else {
                    ch.emplace_back(e.to);
                    ch_data.emplace_back(f_ve(dp_1[e.to], e));
                }
            }
            int n = len(ch);
            if (!n) {
                dp[p] = f_ev(x, p);
                continue;
            }
            vector<Data> prod_left(n, x);
            for(int i = 0; i < n - 1; ++i) prod_left[i + 1] = f_ee(prod_left[i], ch_data[i]);
            Data prod_right = unit;
            for(int i = n - 1; i >= 0; --i) {
                dp_2[ch[i]] = f_ev(f_ee(prod_left[i], prod_right), p);
                prod_right = f_ee(prod_right, ch_data[i]);
            }
            dp[p] = f_ev(f_ee(x, prod_right), p);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_REROOTING