#ifndef AJAY_BIPARTITE_VERTEX_COLORING
#define AJAY_BIPARTITE_VERTEX_COLORING

#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// returns empty vector, if it is not a bipartite graph
template <typename GT>
vector<int> bipartite_vertex_coloring(GT& G) {
    Assert(!GT::is_directed);
    Assert(G.is_prepared());

    int n = G.N;
    UnionFind uf(2 * n);
    for (auto&& e: G.edges) {
        int u = e.frm, v = e.to;
        uf.merge(u + n, v), uf.merge(u, v + n);
    }

    vector<int> color(2 * n, -1);
    For(v, n) if (uf[v] == v && color[uf[v]] < 0) {
        color[uf[v]] = 0;
        color[uf[v + n]] = 1;
    }
    For(v, n) color[v] = color[uf[v]];
    color.resize(n);
    For(v, n) if (uf[v] == uf[v + n]) return {};
    return color;
}
} // namespace mitsuha
#endif // AJAY_BIPARTITE_VERTEX_COLORING