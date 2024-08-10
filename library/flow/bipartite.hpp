#ifndef AJAY_BIPARTITE
#define AJAY_BIPARTITE

#include "library/graph/bipartite_vertex_coloring.hpp"
#include "library/graph/strongly_connected_component.hpp"

namespace mitsuha{
template <typename GT>
struct BipartiteMatching {
    int N;
    GT& G;
    vector<int> color;
    vector<int> dist, match;
    vector<int> vis;

    BipartiteMatching(GT& G) : N(G.N), G(G), dist(G.N, -1), match(G.N, -1) {
        color = bipartite_vertex_coloring(G);
        if (N > 0) assert(!color.empty());
        while (1) {
            bfs();
            vis.assign(N, false);
            int flow = 0;
            For(v, N) if (!color[v] && match[v] == -1 && dfs(v))++ flow;
            if (!flow) break;
        }
    }

    BipartiteMatching(GT& G, vector<int> color)
            : N(G.N), G(G), color(color), dist(G.N, -1), match(G.N, -1) {
        while (1) {
            bfs();
            vis.assign(N, false);
            int flow = 0;
            For(v, N) if (!color[v] && match[v] == -1 && dfs(v))++ flow;
            if (!flow) break;
        }
    }

    void bfs() {
        dist.assign(N, -1);
        queue<int> que;
        For(v, N) if (!color[v] && match[v] == -1) que.emplace(v), dist[v] = 0;
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (auto&& e: G[v]) {
                dist[e.to] = 0;
                int w = match[e.to];
                if (w != -1 && dist[w] == -1) dist[w] = dist[v] + 1, que.emplace(w);
            }
        }
    }

    bool dfs(int v) {
        vis[v] = 1;
        for (auto&& e: G[v]) {
            int w = match[e.to];
            if (w == -1 || (!vis[w] && dist[w] == dist[v] + 1 && dfs(w))) {
                match[e.to] = v, match[v] = e.to;
                return true;
            }
        }
        return false;
    }

    // picking maximum edges st No edges have common vertex
    vector<pair<int, int>> matching() {
        vector<pair<int, int>> res;
        For(v, N) if (v < match[v]) res.emplace_back(v, match[v]);
        return res;
    }

    // https://en.wikipedia.org/wiki/K%C5%91nig%27s_theorem_(graph_theory)#/media/File:Koenigs-theorem-graph.svg
    vector<int> vertex_cover() {
        vector<int> res;
        For(v, N) if (color[v] ^ (dist[v] == -1)) { res.emplace_back(v); }
        return res;
    }

    vector<int> independent_set() {
        vector<int> res;
        For(v, N) if (!(color[v] ^ (dist[v] == -1))) { res.emplace_back(v); }
        return res;
    }

    // use min edges to cover all vertex
    vector<int> edge_cover() {
        vector<bool> done(N);
        vector<int> res;
        for (auto&& e: G.edges) {
            if (done[e.frm] || done[e.to]) continue;
            if (match[e.frm] == e.to) {
                res.emplace_back(e.id);
                done[e.frm] = done[e.to] = 1;
            }
        }
        for (auto&& e: G.edges) {
            if (!done[e.frm]) {
                res.emplace_back(e.id);
                done[e.frm] = 1;
            }
            if (!done[e.to]) {
                res.emplace_back(e.id);
                done[e.to] = 1;
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    /* Dulmage–Mendelsohn decomposition
    https://en.wikipedia.org/wiki/Dulmage%E2%80%93Mendelsohn_decomposition
    http://www.misojiro.t.u-tokyo.ac.jp/~murota/lect-ouyousurigaku/dm050410.pdf
    https://hitonanode.github.io/cplib-cpp/graph/dulmage_mendelsohn_decomposition.hpp.html
    - Maximum matching possible iff with same W
    - Edge uv is always used: Edges with the same W are unique
    - Edge from color=0 to 1: W[l] <= W[r]
    - Points with color=0 are always used: W=1,2,...,K
    - Points with color=1 are always used: W=0,1,...,K-1
    */
    pair<int, vector<int>> DM_decomposition() {
        vector<int> W(N, -1);
        vector<int> que;
        auto add = [&](int v, int x) -> void {
            if (W[v] == -1) {
                W[v] = x;
                que.emplace_back(v);
            }
        };
        For(v, N) if (match[v] == -1 && color[v] == 0) add(v, 0);
        For(v, N) if (match[v] == -1 && color[v] == 1) add(v, iinf);
        while (len(que)) {
            auto v = que.back(); que.pop_back();
            if (match[v] != -1) add(match[v], W[v]);
            if (color[v] == 0 && W[v] == 0) {
                for (auto&& e: G[v]) { add(e.to, W[v]); }
            }
            if (color[v] == 1 && W[v] == iinf) {
                for (auto&& e: G[v]) { add(e.to, W[v]); }
            }
        }

        vector<int> V;
        For(v, N) if (W[v] == -1) V.emplace_back(v);
        int n = len(V);
        Graph<bool, 1> DG(n);
        For(i, n) {
            int v = V[i];
            if (match[v] != -1) {
                int j = lower_bound(V.begin(), V.end(), match[v]) - V.begin();
                DG.add(i, j);
            }
            if (color[v] == 0) {
                for (auto&& e: G[v]) {
                    if (W[e.to] != -1 || e.to == match[v]) continue;
                    int j = lower_bound(V.begin(), V.end(), e.to) - V.begin();
                    DG.add(i, j);
                }
            }
        }
        DG.build();
        auto [K, comp] = strongly_connected_component(DG);
        K += 1;
        For(i, n) { W[V[i]] = 1 + comp[i]; }
        For(v, N) if (W[v] == iinf) W[v] = K;
        return {K, W};
    }
};

template <typename GT>
ostream &operator<<(ostream &out, const BipartiteMatching<GT> &_BM){
    auto BM = _BM;
    out << "match " << BM.match << "\n";
    out << "min vertex covor " << BM.vertex_cover() << "\n";
    out << "max indep set " << BM.independent_set() << "\n";
    out << "min edge cover " << BM.edge_cover();
    return out;
}
} // namespace mitsuha
#endif // AJAY_BIPARTITE