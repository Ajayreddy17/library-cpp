#ifndef AJAY_SUBTREE_DIAMETER
#define AJAY_SUBTREE_DIAMETER

#include "library/graph/tree/rerooting_dp.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree/subtree_diameter.hpp

// https://codeforces.com/problemset/problem/1725/J
template <typename TREE, typename WT>
struct Subtree_Diameter {
    struct Data {
        // diameter found so far: length and endpoints
        WT diam;
        int left, right;
        // longest path from the root: length and endpoint
        WT path;
        int end;
    };

    TREE& tree;
    vector<tuple<int, int, WT>> dp, dp_1, dp_2;

    Subtree_Diameter(TREE& tree) : tree(tree) {
        int N = tree.N;
        Data id = {-1, -1, -1, -1, -1};
        auto f_ee = [&](Data A, Data B) -> Data {
            if (A.diam == -1) return B;
            if (B.diam == -1) return A;
            if (A.diam < B.diam) swap(A, B);
            if (chmax(A.diam, A.path + B.path)) {
                A.left = A.end, A.right = B.end;
            }
            if (chmax(A.path, B.path)) A.end = B.end;
            return A;
        };
        auto f_ev = [&](Data A, int v) -> Data {
            if (A.diam == -1) {
                A.diam = 0, A.left = v, A.right = v, A.path = 0, A.end = v;
            }
            return A;
        };
        auto f_ve = [&](Data A, int r, int nxt_r) -> Data {
            A.path += abs(tree.depth_weighted[r] - tree.depth_weighted[nxt_r]);
            if (chmax(A.diam, A.path)) {
                A.left = nxt_r, A.right = A.end;
            }
            return A;
        };

        Rerooting_DP<TREE, Data> DP(tree, f_ee, f_ev, f_ve, id);
        dp.resize(N), dp_1.resize(N), dp_2.resize(N);
        For(v, N) {
            dp[v] = {DP.dp[v].left, DP.dp[v].right, DP.dp[v].diam};
            dp_1[v] = {
                    DP.dp_subtree[v].left, DP.dp_subtree[v].right, DP.dp_subtree[v].diam};
            dp_2[v] = {
                    DP.dp_parent[v].left, DP.dp_parent[v].right, DP.dp_parent[v].diam};
        }
    }

    // (u, v, diam)
    // full tree rooted at v
    tuple<int, int, WT> operator[](int v) { return dp[v]; }

    // (u, v, diam)
    // subtree v when the tree is rooted at root
    tuple<int, int, WT> get(int v, int root) {
        if (root == v) return dp[v];
        if (!tree.in_subtree(root, v)) {
            return dp_1[v];
        }
        int w = tree.jump(v, root, 1);
        return dp_2[w];
    }
};
} // namespace mitsuha
#endif // AJAY_SUBTREE_DIAMETER
