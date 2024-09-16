#ifndef AJAY_BELLMANFORD
#define AJAY_BELLMANFORD

#include "library/graph/base.hpp"

namespace mitsuha{
// Single start point shortest path. 
// There may be a negative cycle. O(NM) time.
// unreachable: infty<T>
// Can be made as small as desired via negative cycle: -infty<T>
template <typename T, typename GT>
pair<vector<T>, vector<int>> BellmanFord(GT& G, int s) {
    int N = G.N;
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<int> par(N, -1);
    dist[s] = 0;
    int loop = 0;
    while (1) {
        ++loop;
        bool upd = 0;
        For(v, N) {
            if (dist[v] == numeric_limits<T>::max() / 2) continue;
            for (auto&& e: G[v]) {
                T before = dist[e.to];
                T after = dist[v] + e.cost;
                if (dist[v] == -numeric_limits<T>::max() / 2) {
                    after = -numeric_limits<T>::max() / 2;
                }
                chmax(after, -numeric_limits<T>::max() / 2);
                if (before > after) {
                    par[e.to] = v;
                    upd = 1;
                    if (loop >= N) after = -numeric_limits<T>::max() / 2;
                    dist[e.to] = after;
                }
            }
        }
        if (!upd) break;
    }
    return {dist, par};
}
} // namespace mitsuha
#endif // AJAY_BELLMANFORD