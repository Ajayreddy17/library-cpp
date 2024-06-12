#ifndef AJAY_DSU_ON_TREE
#define AJAY_DSU_ON_TREE

#include "library/graph/tree.hpp"

namespace mitsuha{
// add(v) : Add data for vertex v
// query(v) : answer the query at vertex v
// reset(): Return to empty state.
// Depending on the data structure, use history to reset quickly.
// If query at v required without itself, update position of add(v).
template <typename TREE, typename F1, typename F2, typename F3>
void DSU_on_Tree(TREE& tree, F1& add, F2& query, F3& reset) {
    int N = tree.N;
    Frr(vid, N){
        int v = tree.V[vid];

        add(v);
        // collect data in light subtree
        for (auto &&e: tree.G[v]) {
            if (e.to == tree.parent[v]) continue;
            if (tree.head[e.to] != e.to) continue;
            For(idx, tree.LID[e.to], tree.RID[e.to]) { add(tree.V[idx]); }
        }
        query(v);
        if (tree.head[v] == v) reset();
    }
}
} // namespace mitsuha
#endif // AJAY_DSU_ON_TREE