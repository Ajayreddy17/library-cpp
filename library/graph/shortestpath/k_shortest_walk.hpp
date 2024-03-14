#ifndef AJAY_K_SHORTEST_WALK
#define AJAY_K_SHORTEST_WALK

#include "library/graph/shortestpath/dijkstra.hpp"
#include "library/graph/reverse_graph.hpp"
#include "library/datastructure/meldable_heap.hpp"
namespace mitsuha{
// infty Returns a padded version that is always of length K.
template <typename T, typename GT, int NODES>
vector<T> K_shortest_walk(GT &G, int s, int t, int K) {
    static_assert(GT::is_directed);
    int N = G.N;
    auto RG = reverse_graph(G);
    auto [dist, par] = dijkstra<T, decltype(RG)>(RG, t);
    if (dist[s] == numeric_limits<T>::max() / 2) { return vector<T>(K, numeric_limits<T>::max() / 2); }

    using P = pair<T, int>;
    Meldable_Heap<P, true, NODES, true> X;
    using np = typename decltype(X)::np;
    vector<np> nodes(N, nullptr);

    vector<bool> vis(N);
    vector<int> st = {t};
    vis[t] = true;
    while (len(st)) {
        int v = st.back(); st.pop_back();
        bool done = 0;
        for (auto &&e: G[v]) {
            if (dist[e.to] == numeric_limits<T>::max() / 2) continue;
            if (!done && par[v] == e.to && dist[v] == dist[e.to] + e.cost) {
                done = true;
                continue;
            }
            T cost = -dist[v] + e.cost + dist[e.to];
            nodes[v] = X.push(nodes[v], {cost, e.to});
        }
        for (auto &&e: RG[v]) {
            if (vis[e.to]) continue;
            if (par[e.to] == v) {
                nodes[e.to] = X.meld(nodes[e.to], nodes[v]);
                vis[e.to] = 1;
                st.emplace_back(e.to);
            }
        }
    }

    T base = dist[s];
    vector<T> ANS = {base};
    if (nodes[s]) {
        using PAIR = pair<T, np>;
        auto comp = [](auto a, auto b) { return a.first > b.first; };
        priority_queue<PAIR, vector<PAIR>, decltype(comp)> que(comp);
        que.emplace(base + X.top(nodes[s]).first, nodes[s]);
        while (len(ANS) < K && len(que)) {
            auto [d, n] = que.top();
            que.pop();
            ANS.emplace_back(d);
            if (n->l) que.emplace(d + (n->l->x.first) - (n->x.first), n->l);
            if (n->r) que.emplace(d + (n->r->x.first) - (n->x.first), n->r);
            np m = nodes[n->x.second];
            if (m) { que.emplace(d + m->x.first, m); }
        }
    }
    while (len(ANS) < K) ANS.emplace_back(numeric_limits<T>::max() / 2);
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_K_SHORTEST_WALK