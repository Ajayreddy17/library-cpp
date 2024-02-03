#ifndef AJAY_COMPL_GRAPH_BFS
#define AJAY_COMPL_GRAPH_BFS

#include "library/graph/base.hpp"

namespace mitsuha{
template <typename GT>
pair<vector<int>, vector<int>> complement_graph_bfs(GT& G, int s) {
    static vector<int> que, NG, dist, par, yet;
    const int N = G.N;
    if (len(que) < N) que.resize(N);
    if (len(NG) < N) NG.resize(N);
    if (len(yet) < N) yet.resize(N);
    dist.assign(N, iinf);
    par.assign(N, -1);
    int ql = 0, qr = 0;
    dist[s] = 0, que[qr++] = s;
    int p = 0;
    For(v, N) if (v != s) yet[p++] = v;
    while (ql < qr) {
        int v = que[ql++];
        for (auto& e: G[v]) NG[e.to] = 1;
        for (int i = p - 1; i >= 0; --i) {
            int to = yet[i];
            if (NG[to]) continue;
            dist[to] = dist[v] + 1, par[to] = v, que[qr++] = to;
            swap(yet[i], yet[--p]);
        }
        for (auto& e: G[v]) NG[e.to] = 0;
    }
    return {dist, par};
}
} // namespace mitsuha
#endif // AJAY_COMPL_GRAPH_BFS