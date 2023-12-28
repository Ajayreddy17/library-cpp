#ifndef AJAY_BFS01
#define AJAY_BFS01

#include "library/graph/base.hpp"

namespace mitsuha{
// [dist, par]
template <typename T, typename GT>
pair<vector<T>, vector<int>> bfs01(GT& G, int v) {
    assert(G.is_prepared());
    int N = G.N;
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<int> par(N, -1);
    deque<int> que;

    dist[v] = 0;
    que.push_front(v);
    while (!que.empty()) {
        auto v = que.front();
        que.pop_front();
        for (auto &&e: G[v]) {
            if (dist[e.to] == numeric_limits<T>::max() / 2 or dist[e.to] > dist[e.frm] + e.cost) {
                dist[e.to] = dist[e.frm] + e.cost;
                par[e.to] = e.frm;
                if (e.cost == 0)
                    que.push_front(e.to);
                else
                    que.push_back(e.to);
            }
        }
    }
    return {dist, par};
}

// [dist, par, root]
template <typename T, typename GT>
tuple<vector<T>, vector<int>, vector<int>> bfs01(GT& G, vector<int> vs) {
    assert(G.is_prepared());
    int N = G.N;
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<int> par(N, -1);
    vector<int> root(N, -1);
    deque<int> que;

    for (auto &&v: vs) {
        dist[v] = 0;
        root[v] = v;
        que.push_front(v);
    }

    while (!que.empty()) {
        auto v = que.front();
        que.pop_front();
        for (auto &&e: G[v]) {
            if (dist[e.to] == numeric_limits<T>::max() / 2 || dist[e.to] > dist[e.frm] + e.cost) {
                dist[e.to] = dist[e.frm] + e.cost;
                root[e.to] = root[e.frm];
                par[e.to] = e.frm;
                if (e.cost == 0)
                    que.push_front(e.to);
                else
                    que.push_back(e.to);
            }
        }
    }
    return {dist, par, root};
}
} // namespace mitsuha
#endif // AJAY_BFS01