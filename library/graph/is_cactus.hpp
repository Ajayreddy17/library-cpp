#ifndef AJAY_IS_CACTUS
#define AJAY_IS_CACTUS

#include "library/graph/block_cut.hpp"
#include "library/graph/tree.hpp"
#include "library/graph/two_edge_component.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/is_cactus.hpp


// EDGE = true: every edge is on at most one cycle (cycles or edges meeting at articulation points)
// EDGE = false: every vertex is on at most one cycle (cycles or edges joined by bridges)
template <bool EDGE>
bool is_cactus(Graph<int, 0> G) {
    if constexpr (EDGE) {
        static_assert(EDGE);
        Block_Cut X(G);
        int N = G.N, n = X.BCT.N;
        Tree<decltype(X.BCT)> tree(X.BCT);
        vector<int> NV(n), NE(n);
        For(i, N, n) NV[i] = X.BCT.deg(i);
        for (auto& e : G.edges) {
            int a = e.frm, b = e.to;
            int i = tree.jump(a, b, 1);
            NE[i]++;
        }
        For(i, N, n) {
            int a = NV[i], b = NE[i];
            if (b == 1) continue;
            if (a < b) return false;
        }
        return true;
    } else {
        auto [nc, comp] = two_edge_component(G);
        vector<int> NV(nc), NE(nc);
        For(v, G.N) NV[comp[v]]++;
        for (auto& e : G.edges) {
            int a = e.frm, b = e.to;
            a = comp[a], b = comp[b];
            if (a == b) NE[a]++;
        }
        For(i, nc) {
            if (NV[i] < NE[i]) return false;
        }
        return true;
    }
}
} // namespace mitsuha
#endif // AJAY_IS_CACTUS
