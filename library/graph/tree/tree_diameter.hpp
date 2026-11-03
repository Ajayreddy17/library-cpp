#ifndef AJAY_TREE_DIAMETER
#define AJAY_TREE_DIAMETER

#include "library/graph/shortestpath/bfs01.hpp"
#include "library/graph/restore_path.hpp"

namespace mitsuha{
// length, path
template <typename T, typename GT>
pair<T, vector<int>> tree_diameter(GT& G) {
    assert(G.is_prepared());
    int A, B;
    auto [distA, parA] = bfs01<T>(G, 0);
    A = max_element(distA.begin(), distA.end()) - distA.begin();
    auto [dist, par] = bfs01<T>(G, A);
    B = max_element(dist.begin(), dist.end()) - dist.begin();
    vector<int> P = restore_path(par, B);
    return {dist[B], P};
}
} // namespace mitsuha
#endif // AJAY_TREE_DIAMETER
