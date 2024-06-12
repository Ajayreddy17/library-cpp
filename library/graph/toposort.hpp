#ifndef AJAY_TOPOSORT
#define AJAY_TOPOSORT

#include "library/graph/base.hpp"
#include "library/datastructure/fastset.hpp"

namespace mitsuha{
// Return the lexicographically smallest toposort
template <typename GT>
vector<int> toposort(GT& G) {
    assert(G.is_prepared() && GT::is_directed);
    const int N = G.N;
    auto [indeg, outdeg] = G.deg_array_inout();
    FastSet que(N);
    vector<int> V;
    for(int v = 0; v < N; v++) if (indeg[v] == 0) que.insert(v);
    while (1) {
        int v = que.next(0);
        if (v == N) break;
        que.erase(v), V.emplace_back(v);
        for (auto&& e: G[v]) {
            if (--indeg[e.to] == 0) que.insert(e.to);
        }
    }
    return ((int)(V.size()) < N ? vector<int>{} : V);
}
} // namespace mitsuha
#endif // AJAY_TOPOSORT