#ifndef AJAY_K_SHORTEST_PATH
#define AJAY_K_SHORTEST_PATH

#include "library/graph/base.hpp"

namespace mitsuha{
// (cost, vs, es)
template <typename T, typename GT>
vector<tuple<T, vector<int>, vector<int>>> K_shortest_path(GT& G, int s, int t, int K) {
    assert(GT::is_directed);
    const int N = G.N;

    // (cost, vs, es)
    vector<tuple<T, vector<int>, vector<int>>> res;
    // Starting point of exploration(es, ng_edges)
    vector<pair<vector<int>, vector<int>>> nodes;
    // possible answers (cost, es, ng_es, n), (ng_es, n)：Conditions for finding that path
    vector<tuple<T, vector<int>, vector<int>, int>> paths;

    nodes.emplace_back(vector<int>(), vector<int>());
    vector<T> dist(N, numeric_limits<T>::max() / 2);
    vector<bool> ng_v(N);
    vector<bool> ng_e(G.M);
    vector<int> par(N, -1);

    while (len(res) < K) {
        for (auto&& [es, ng_es]: nodes) {
            fill(par.begin(), par.end(), -1);
            fill(ng_v.begin(), ng_v.end(), 0);
            fill(ng_e.begin(), ng_e.end(), 0);
            fill(dist.begin(), dist.end(), numeric_limits<T>::max() / 2);

            T pref_cost = 0;
            for (auto&& x: es) pref_cost += G.edges[x].cost;

            for (auto&& x: es) ng_v[G.edges[x].frm] = 1, ng_e[x] = 1;
            for (auto&& x: ng_es) ng_e[x] = 1;
            // dijkstra
            priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;
                auto add = [&](int v, T d, int p) -> void {
                    if (chmin(dist[v], d)) {
                    que.emplace(d, v);
                    par[v] = p;
                }
            };
            int s0 = (es.empty() ? s : G.edges[es.back()].to);
            add(s0, pref_cost, -1);
            while (len(que)) {
                auto [dv, v] = que.top(); que.pop();
                if (dv != dist[v]) continue;
                if (v == t) break;
                for (auto&& e: G[v]) {
                    if (ng_e[e.id] || ng_v[e.to]) continue;
                    add(e.to, dv + e.cost, e.id);
                }
            }
            // failed
            if (par[t] == -1) continue;
            // restore path
            vector<int> add_e;
            {
                int v = t;
                while (v != s0) {
                    add_e.emplace_back(par[v]);
                    v = G.edges[par[v]].frm;
                }
            }
            reverse(add_e.begin(), add_e.end());
            int n = len(es);
            // find a path
            es.insert(es.end(), add_e.begin(), add_e.end());
            paths.emplace_back(dist[t], es, ng_es, n);
        }
        // choose best path
        if (len(paths) == 0) break;
        pair<int, T> best = {-1, numeric_limits<T>::max() / 2};
        For(i, len(paths)) {
            T cost = get<0>(paths[i]);
            if (chmin(best.second, cost)) best.first = i;
        }
        int idx = best.first;
        swap(paths[idx], paths[len(paths) - 1]);
        auto [cost, es, ng_es, n] = paths.back(); paths.pop_back();
        vector<int> vs = {s};
        for (auto&& x: es) vs.emplace_back(G.edges[x].to);
        res.emplace_back(cost, vs, es);

        nodes.clear();
        For(k, n, len(es)) {
            // k Fix up to the th side
            vector<int> new_es = {es.begin(), es.begin() + k};
            vector<int> new_ng = ng_es;
            new_ng.emplace_back(es[k]);
            nodes.emplace_back(new_es, new_ng);
        }
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_K_SHORTEST_PATH