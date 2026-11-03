#ifndef AJAY_GOMORY_HU_TREE
#define AJAY_GOMORY_HU_TREE

#include "library/graph/base.hpp"
#include "library/flow/maxflow.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/gomory_hu_tree.hpp
// Undirected G -> tree whose path minima are the s-t min cuts of G.
// assumes every cut is < numeric_limits<T>::max() / 2
template <typename T>
Graph<T, 0> gomory_hu_tree(const Graph<T, 0> &G) {
    int N = G.N;
    vector<int> par(N, -1);
    vector<T> wt(N);
    For(v, 1, N) par[v] = 0;

    For(s, 1, N) {
        int t = par[s];
        MaxFlow<T> F(N, s, t);
        for (auto &e : G.edges) {
            F.add(e.frm, e.to, e.cost, e.cost);
        }
        auto [val, cut] = F.cut();
        wt[s] = val;

        For(v, 1, N) {
            if (v != s && par[v] == t && cut[v] == 0) par[v] = s;
        }
        if (par[t] != -1 && cut[par[t]] == 0) {
            par[s] = par[t], par[t] = s, wt[s] = wt[t], wt[t] = val;
        }
    }
    Graph<T, 0> ANS(N);
    For(v, 1, N) ANS.add(par[v], v, wt[v]);
    ANS.build();
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_GOMORY_HU_TREE
