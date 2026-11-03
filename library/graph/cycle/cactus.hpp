#ifndef AJAY_CACTUS
#define AJAY_CACTUS

#include "library/datastructure/csr.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/cycle/cactus.hpp


// block index -> its vertex list
CSR<int> get_cactus_cycles(
        Graph<int, 0>& G, Graph<int, 0>& BCT, Tree<Graph<int, 0>>& tree) {
    CSR<pair<int, int>> es(BCT.N);
    CSR<int> cs(BCT.N);
    for (auto& e : G.edges) {
        int a = e.frm, b = e.to;
        int k = tree.jump(a, b, 1);
        es.add(k, {a, b});
    }
    es.build();
    vector<int> SM(G.N);

    vector<pair<int, int>> E;
    vector<int> C;
    For(k, G.N, BCT.N) {
        E.clear();
        for (auto& [a, b] : es[k]) E.emplace_back(a, b);
        if (len(E) == 1) continue;
        for (auto& [a, b] : E) SM[a] += b, SM[b] += a;
        C.clear();
        C.emplace_back(E[0].first);
        C.emplace_back(E[0].second);
        while (C.back() != C[0]) {
            int a = C[len(C) - 2], b = C[len(C) - 1];
            C.emplace_back(SM[b] - a);
        }
        POP(C);
        for (int v : C) cs.add(k, v);
        for (auto& [a, b] : E) SM[a] -= b, SM[b] -= a;
    }
    cs.build();
    return cs;
}
} // namespace mitsuha
#endif // AJAY_CACTUS
