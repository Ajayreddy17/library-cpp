#ifndef AJAY_COMPRESS_TREE
#define AJAY_COMPRESS_TREE

#include "library/graph/tree.hpp"

namespace mitsuha{
// (compressed tree vertex labels, graph)
// New graph: with edge weights
template <typename TREE>
pair<vector<int>, typename TREE::Graph_type> compress_tree(TREE &tree, vector<int> V){
    // List important points
    // Make sure the original roots are included
    sort(V.begin(), V.end(), [&](auto &x, auto &y){ return tree.LID[x] < tree.LID[y]; });
    int n = len(V);
    For(i, n){
        int j = (i + 1 == n ? 0 : i + 1);
        V.emplace_back(tree.lca(V[i], V[j]));
    }
    V.emplace_back(tree.V[0]);
    sort(V.begin(), V.end(), [&](auto &x, auto &y){ return tree.LID[x] < tree.LID[y]; });
    V.erase(unique(V.begin(), V.end()), V.end());
    // Create a graph by stretching the edges
    n = len(V);
    using GT = typename TREE::Graph_type;
    using WT = typename GT::cost_type;
    GT G(n);
    vector<int> st = {0};
    For(i, 1, n){
        while (1){
            int p = V[st.back()];
            int v = V[i];
            if (tree.in_subtree(v, p))
                break;
            st.pop_back();
        }
        int p = V[st.back()];
        int v = V[i];
        WT d = tree.depth_weighted[v] - tree.depth_weighted[p];
        G.add(st.back(), i, d);
        st.emplace_back(i);
    }
    G.build();
    return {V, G};
}
} // namespace mitsuha
#endif // AJAY_COMPRESS_TREE