#ifndef AJAY_COMPRESS_TREE
#define AJAY_COMPRESS_TREE

#include "library/graph/tree.hpp"

namespace mitsuha {
// returns (graph_vertex to tree_vertex, compressed_graph)
// graph has edge weights [depth_weighted of tree]
template <typename TREE>
pair<typename TREE::Graph_type, vector<int>> compress_tree(TREE &tree, vector<int> V){
    sort(V.begin(), V.end(), [&](auto &x, auto &y){ return tree.LID[x] < tree.LID[y]; });
    int n = len(V);
    For(i, n){
        int j = (i + 1 == n ? 0 : i + 1);
        V.emplace_back(tree.lca(V[i], V[j]));
    }
    sort(V.begin(), V.end(), [&](auto &x, auto &y){ return tree.LID[x] < tree.LID[y]; });
    V.erase(unique(V.begin(), V.end()), V.end());
    n = len(V);
    using GT = typename TREE::Graph_type;
    using WT = typename GT::cost_type;
    GT G(n);
    vector<int> st = {0};
    For(i, 1, n){
        while (true){
            int p = V[st.back()];
            int v = V[i];
            if (tree.in_subtree(v, p)) break;
            st.pop_back();
        }
        int p = V[st.back()];
        int v = V[i];
        WT d = tree.depth_weighted[v] - tree.depth_weighted[p];
        G.add(st.back(), i, d);
        st.emplace_back(i);
    }
    G.build();
    return {G, V};
}
} // namespace mitsuha
#endif // AJAY_COMPRESS_TREE