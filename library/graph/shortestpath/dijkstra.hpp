#ifndef AJAY_DIJKSTRA
#define AJAY_DIJKSTRA

#include "library/graph/base.hpp"

namespace mitsuha{
template <typename T, typename GT>
pair<vector<T>, vector<int>> dijkstra_dense(GT& G, int s) {
    const int N = G.N;
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<int> par(N, -1);
    vector<bool> done(N);
    dist[s] = 0;
    while (1) {
        int v = -1;
        T mi = numeric_limits<T>::max() / 2;
        for(int i = 0; i < N; i++) {
            if (!done[i] && chmin(mi, dist[i])) v = i;
        }
        if (v == -1) break;
        done[v] = 1;
        for (auto&& e: G[v]) {
            if (chmin(dist[e.to], dist[v] + e.cost)) par[e.to] = v;
        }
    }
    return {dist, par};
}

template <typename T, typename GT, bool DENSE = false>
pair<vector<T>, vector<int>> dijkstra(GT& G, int v) {
    if (DENSE) return dijkstra_dense<T>(G, v);
    auto N = G.N;
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<int> par(N, -1);
    using P = pair<T, int>;

    priority_queue<P, vector<P>, greater<P>> que;

    dist[v] = 0;
    que.emplace(0, v);
    while (!que.empty()) {
        auto [dv, v] = que.top();
        que.pop();
        if (dv > dist[v]) continue;
        for (auto&& e: G[v]) {
            if (chmin(dist[e.to], dist[e.frm] + e.cost)) {
                par[e.to] = e.frm;
                que.emplace(dist[e.to], e.to);
            }
        }
    }
    return {dist, par};
}

// Multi source. [dist, par, root]
template <typename T, typename GT>
tuple<vector<T>, vector<int>, vector<int>> dijkstra(GT& G, vector<int> vs) {
    assert(G.is_prepared());
    int N = G.N;
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<int> par(N, -1);
    vector<int> root(N, -1);

    using P = pair<T, int>;

    priority_queue<P, vector<P>, greater<P>> que;

    for (auto&& v: vs) {
        dist[v] = 0;
        root[v] = v;
        que.emplace(T(0), v);
    }

    while (!que.empty()) {
        auto [dv, v] = que.top();
        que.pop();
        if (dv > dist[v]) continue;
        for (auto&& e: G[v]) {
            if (chmin(dist[e.to], dist[e.frm] + e.cost)) {
                root[e.to] = root[e.frm];
                par[e.to] = e.frm;
                que.emplace(dist[e.to], e.to);
            }
        }
    }
    return {dist, par, root};
}
} // namespace mitsuha
#endif // AJAY_DIJKSTRA