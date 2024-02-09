#ifndef AJAY_MINIMUM_SPANNING_TREE
#define AJAY_MINIMUM_SPANNING_TREE

#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/graph/tree.hpp"
#include "library/graph/datastructure/tree_monoid.hpp"
#include "library/graph/datastructure/dual_tree_monoid.hpp"
#include "library/monoid/monoid_min.hpp"
#include "library/monoid/monoid_max.hpp"

namespace mitsuha{
// return : {T mst_cost, vc<bool> in_mst, Graph MST}
template <typename T, typename GT>
tuple<T, vector<bool>, GT> minimum_spanning_tree(GT& G) {
    int N = G.N;
    int M = len(G.edges);
    vector<int> I(M);
    For(i, M) I[i] = i;
    sort(I.begin(), I.end(), [&](auto& a, auto& b) -> bool { return (G.edges[a].cost) < (G.edges[b].cost); });

    vector<bool> in_mst(M);
    UnionFind uf(N);
    T mst_cost = T(0);
    GT MST(N);
    for (auto& i: I) {
        auto& e = G.edges[i];
        if (uf.merge(e.frm, e.to)) {
            in_mst[i] = 1;
            mst_cost += e.cost;
        }
    }
    For(i, M) if (in_mst[i]) {
        auto& e = G.edges[i];
        MST.add(e.frm, e.to, e.cost);
    }
    MST.build();
    return {mst_cost, in_mst, MST};
}

// https://codeforces.com/contest/828/problem/F
// return : {T mst_cost, vc<bool> in_mst, Graph MST, vc<T> dat}
// dat : For each edge, the maximum weight that would result in an MST edge if the other edges were kept
template <typename T, typename GT>
tuple<T, vector<bool>, GT, vector<T>> minimum_spanning_tree_cycle_data(GT& G) {
    int M = len(G.edges);
    auto [mst_cost, in_mst, MST] = minimum_spanning_tree<T>(G);
    Tree<GT> tree(MST);
    vector<T> dat;
    For(i, M) if (in_mst[i]) dat.emplace_back(G.edges[i].cost);
    Tree_Monoid<decltype(tree), Monoid_Max<T>, 1> TM1(tree, dat);
    Dual_Tree_Monoid<decltype(tree), Monoid_Min<T>, 1> TM2(tree);
    For(i, M) {
        if (!in_mst[i]) {
            auto& e = G.edges[i];
            TM2.apply_path(e.frm, e.to, e.cost);
        }
    }
    vector<T> ANS(M);
    int m = 0;
    For(i, M) {
        auto& e = G.edges[i];
        if (in_mst[i]) ANS[i] = TM2.get(m++);
        else ANS[i] = TM1.prod_path(e.frm, e.to);
    }
    return {mst_cost, in_mst, MST, ANS};
}
} // namespace mitsuha
#endif // AJAY_MINIMUM_SPANNING_TREE