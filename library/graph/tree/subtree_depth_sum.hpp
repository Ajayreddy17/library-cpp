#ifndef AJAY_SUBTREE_DEPTH_SUM
#define AJAY_SUBTREE_DEPTH_SUM

#include "library/graph/tree/rerooting_dp.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree/subtree_depth_sum.hpp


// sum_v dist(root, v)
template <typename TREE, typename WT = long long>
struct Subtree_Depth_Sum {
    // num_point, dist_sum
    using Data = pair<int, WT>;
    TREE& tree;
    vector<Data> dp, dp_1, dp_2;

    Subtree_Depth_Sum(TREE& tree) : tree(tree) {
        Data id = {0, 0};
        auto f_ee = [&](Data A, Data B) -> Data {
            return {A.first + B.first, A.second + B.second};
        };
        auto f_ev = [&](Data A, int) -> Data { return {A.first + 1, A.second}; };
        auto f_ve = [&](Data A, int r, int nxt_r) -> Data {
            WT x = abs(tree.depth_weighted[r] - tree.depth_weighted[nxt_r]);
            return {A.first, A.second + A.first * x};
        };

        Rerooting_DP<TREE, Data> DP(tree, f_ee, f_ev, f_ve, id);
        dp = DP.dp, dp_1 = DP.dp_subtree, dp_2 = DP.dp_parent;
    }

    // (cnt, sum)
    // full tree rooted at v
    pair<int, WT> operator[](int v) { return dp[v]; }

    // (cnt, sum)
    // subtree v when the tree is rooted at root
    pair<int, WT> get(int v, int root) {
        if (root == v) return dp[v];
        if (!tree.in_subtree(root, v)) {
            return dp_1[v];
        }
        int w = tree.jump(v, root, 1);
        return dp_2[w];
    }
};
} // namespace mitsuha
#endif // AJAY_SUBTREE_DEPTH_SUM
