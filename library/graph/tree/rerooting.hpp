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

    template <typename RAKE, typename APPLY_VERTEX, 
                                typename APPLY_EDGE>
    Rerooting_dp(TREE& tree, RAKE rake, APPLY_VERTEX apply_vertex, 
                            APPLY_EDGE apply_edge, const Data unit)
            : tree(tree) {
        build(rake, apply_vertex, apply_edge, unit);
    }
    
    Data operator[](int v) { return dp[v]; }

    // Subtree v when root is the root
    Data get(int v, int root) {
        if (root == v) return dp[v];
        if (!tree.in_subtree(root, v)) { return dp_1[v]; }
        int w = tree.jump(v, root, 1);
        return dp_2[w];
    }

    template <typename RAKE, typename APPLY_VERTEX, 
                                typename APPLY_EDGE>
    void build(RAKE rake, APPLY_VERTEX apply_vertex, 
                    APPLY_EDGE apply_edge, const Data unit) {
        int N = tree.N;
        // dp1: subtree
        dp_1.assign(N, unit);
        Frr(i, N) {
            int v = tree.V[i];
            for (auto&& e: tree.G[v]) {
                if (e.to == tree.parent[v]) continue;
                dp_1[v] = rake(dp_1[v], apply_edge(dp_1[e.to], e));
            }
            dp_1[v] = apply_vertex(dp_1[v], v);
        }

        // dp2[v]: subtree of p, rooted at v
        dp_2.assign(N, unit);
        // dp[v]: fulltree, rooted at v
        dp.assign(N, unit);
        For(i, N) {
            int p = tree.V[i];
            vector<int> ch;
            vector<Data> ch_data;
            Data x = unit;
            for (auto&& e: tree.G[p]) {
                if (e.to == tree.parent[p]) {
                    x = apply_edge(dp_2[p], e);
                } else {
                    ch.emplace_back(e.to);
                    ch_data.emplace_back(apply_edge(dp_1[e.to], e));
                }
            }
            int n = len(ch);
            if (!n) {
                dp[p] = apply_vertex(x, p);
                continue;
            }
            vector<Data> prod_left(n, x);
            For(i, n - 1) prod_left[i + 1] = rake(prod_left[i], ch_data[i]);
            Data prod_right = unit;
            Frr(i, n) {
                dp_2[ch[i]] = apply_vertex(rake(prod_left[i], prod_right), p);
                prod_right = rake(prod_right, ch_data[i]);
            }
            dp[p] = apply_vertex(rake(x, prod_right), p);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_REROOTING