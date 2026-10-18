#ifndef AJAY_TOP2_DIJKSTRA
#define AJAY_TOP2_DIJKSTRA

#include "library/graph/base.hpp"
#include "library/monoid/monoid_min2.hpp"

namespace mitsuha{
// It only has the distance and type, without path reconstruction
template <typename T, typename GT>
vector<typename Monoid_Min2<T, int>::Data> top2_dijkstra(GT& G, vector<int> vs) {
    assert(G.is_prepared());
    int N = G.N;
    using Mono = Monoid_Min2<T, int>;
    using Data = typename Mono::Data;
    vector<Data> dist(N, Mono::unit());
    min_priority_queue<tuple<T, int, int>> que; // dist, node, color

    auto upd = [&](int v, int c, T x) -> void {
        if (dist[v].add_element(x, c)) que.emplace(x, v, c);
    };

    for (auto& v : vs) {
        upd(v, v, 0);
    }

    while (not que.empty()) {
        auto [dv, v, c] = que.top(); que.pop();
        bool ok = 0;
        auto& e = dist[v];
        if (e.min1 == dv && e.key1 == c) ok = 1;
        if (e.min2 == dv && e.key2 == c) ok = 1;
        if (!ok) continue;
        for (auto&& e : G[v]) {
            upd(e.to, c, dv + e.cost);
        }
    }
    return dist;
}
} // namespace mitsuha
#endif // AJAY_TOP2_DIJKSTRA