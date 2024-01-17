#ifndef AJAY_WARSHALL_FLOYD
#define AJAY_WARSHALL_FLOYD

#include "library/graph/base.hpp"

namespace mitsuha{
// Even if there is a negative side, it will work correctly if there is no negative cycle.
// Whether there is a negative cycle is determined by whether there is a v such that dist[v][v] < 0.
template <typename T, typename GT>
vector<vector<T>> warshall_floyd(GT& G) {
    long long N = G.N;
    vector<vector<T>> dist(N, vector<T>(N, numeric_limits<T>::max() / 2));
    For(v, N) {
        dist[v][v] = 0;
        for (auto&& e: G[v]) chmin(dist[v][e.to], e.cost);
    }
    For(k, N) For(i, N) {
        if (dist[i][k] == numeric_limits<T>::max() / 2) continue;
        For(j, N) {
            if (dist[k][j] == numeric_limits<T>::max() / 2) continue;
            chmin(dist[i][j], dist[i][k] + dist[k][j]);
        }
    }
    return dist;
}
} // namespace mitsuha
#endif // AJAY_WARSHALL_FLOYD