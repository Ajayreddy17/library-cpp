#ifndef AJAY_TREE_CENTER
#define AJAY_TREE_CENTER

#include "library/graph/shortestpath/bfs01.hpp"
#include "library/graph/restore_path.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_center.hpp

// not verified
// {a, b, d}: move d from a towards b / {a, a, 0}
// assumes the diameter is even (double the weights if needed)
template <typename T, typename GT>
tuple<int, int, T> tree_center(GT& G) {
    assert(G.is_prepared());
    auto [distA, parA] = bfs01<T>(G, 0);
    int a = max_element(distA.begin(), distA.end()) - distA.begin();
    auto [dist, par] = bfs01<T>(G, a);
    int b = max_element(dist.begin(), dist.end()) - dist.begin();
    T d = dist[b];
    assert(d % 2 == 0);
    vector<int> path = restore_path(par, b);
    For(i, len(path)) {
        int v = path[i];
        if (dist[v] == d / 2) return {v, v, 0};
    }
    For(i, len(path) - 1) {
        int a = path[i], b = path[i + 1];
        if (dist[a] < d / 2 && d / 2 < dist[b]) {
            return {a, b, d / 2 - dist[a]};
        }
    }
    assert(0);
    return {-1, -1, 0};
}
} // namespace mitsuha
#endif // AJAY_TREE_CENTER
