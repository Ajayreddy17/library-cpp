#ifndef AJAY_SUBTREE_HASH
#define AJAY_SUBTREE_HASH

#include "library/mod/modint61.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree.hpp"
#include "library/random/base.hpp"
#include "library/graph/tree_dp/rerooting_dp.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_dp/subtree_hash.hpp

// safe to use across several trees
template <typename TREE>
struct Subtree_Hash {
    using mint = modint61;
    TREE& tree;
    vector<uint64_t> dp, dp_subtree, dp_parent;

    Subtree_Hash(TREE& tree) : tree(tree) {
        int N = tree.N;
        using T = pair<int, mint>;
        T id = {0, mint(1)};

        auto f_ee = [&](T A, T B) -> T { return {max(A.first, B.first), A.second * B.second}; };
        auto f_ev = [&](T A, int) -> T { return {A.first + 1, A.second}; };
        auto f_ve = [&](T A, int, int) -> T {
            return {A.first, A.second + hash_base(A.first)};
        };

        Rerooting_DP<TREE, T> DP(tree, f_ee, f_ev, f_ve, id);

        dp.resize(N), dp_subtree.resize(N), dp_parent.resize(N);
        For(v, N) {
            dp[v] = DP.dp[v].second.val;
            dp_subtree[v] = DP.dp_subtree[v].second.val;
            dp_parent[v] = DP.dp_parent[v].second.val;
        }
    }

    // full tree rooted at v
    uint64_t operator[](int v) const { return dp[v]; }

    // subtree v when the tree is rooted at root
    uint64_t get(int v, int root) const {
        if (root == v) return dp[v];
        if (!tree.in_subtree(root, v)) return dp_subtree[v];
        int w = tree.jump(v, root, 1);
        return dp_parent[w];
    }

    static mint hash_base(int k) {
        static vector<mint> dat;
        while (len(dat) <= k) dat.emplace_back(RNG(mint::get_mod()));
        return dat[k];
    }
};
} // namespace mitsuha
#endif // AJAY_SUBTREE_HASH
