#ifndef AJAY_RANK_MAXIMAL_BIPARTITE_MATCHING
#define AJAY_RANK_MAXIMAL_BIPARTITE_MATCHING

#include "library/graph/bipartite_vertex_coloring.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/rank_maximal_bipartite_matching.hpp


// (N1, N2) bipartite graph with edge-weight 0, 1, ..., K - 1.
// find a matching s.t. (# of K-1, # of K-2, ...) is lex max.
// https://yukicoder.me/problems/no/1615
// https://qoj.ac/contest/1388/problem/6546

template <typename GT>
struct Rank_Maximal_Bipartite_Matching {
    int N, K;
    GT& G;
    vector<int> color;
    vector<int> dist, match;
    vector<int> que;
    vector<bool> vis;
    vector<bool> vcover;

    // edge bookkeeping
    // [L,M) : active, [M,R) : inactive
    vector<pair<int, int>> dat;
    vector<int> LID, MID, RID;

    Rank_Maximal_Bipartite_Matching(GT& G) : N(G.N), G(G) {
        color = bipartite_vertex_coloring(G, true);
        if (N > 0) assert(!color.empty());
        dist.assign(N, -1), match.assign(N, -1);
        que.assign(N, -1), vis.assign(N, -1);
        vcover.assign(N, 0);
        K = 0;
        for (auto& e : G.edges) chmax(K, e.cost);
        ++K;
        build();
        Frr(k, K) solve(k);
    }

    void build() {
        For(v, N) {
            LID.emplace_back(len(dat));
            if (color[v] == 0) {
                for (auto& e : G[v]) {
                    dat.emplace_back(e.to, e.cost);
                }
            }
        }
        LID.emplace_back(len(dat));
        MID.resize(N), RID.resize(N);
        For(v, N) MID[v] = LID[v], RID[v] = LID[v + 1];
    }

    void solve(int k) {
        // activate edges of weight k
        For(v, N) {
            if (vcover[v]) continue;
            For(i, MID[v], RID[v]) {
                auto [to, cost] = dat[i];
                if (cost != k) continue;
                swap(dat[MID[v]], dat[i]), ++MID[v];
            }
        }
        while (1) {
            bfs();
            vis.assign(N, false);
            int flow = 0;
            For(v, N) if (!color[v] && match[v] == -1 && dfs(v))++ flow;
            if (!flow) break;
        }

        // update vertex cover
        For(v, N) { vcover[v] = (color[v] ^ (dist[v] == -1)); }
        // delete active edges with both ends touching the vertex cover
        For(v, N) {
            if (!vcover[v]) continue;
            Frr(i, LID[v], MID[v]) {
                auto [to, cost] = dat[i];
                if (vcover[to]) {
                    swap(dat[i], dat[MID[v] - 1]);
                    swap(dat[MID[v] - 1], dat[RID[v] - 1]);
                    --MID[v], --RID[v];
                }
            }
        }
        // delete inactive edges with at least one end touching the vertex cover
        For(v, N) {
            if (vcover[v]) {
                RID[v] = MID[v];
                continue;
            }
            Frr(i, MID[v], RID[v]) {
                auto [to, cost] = dat[i];
                if (vcover[to]) {
                    swap(dat[i], dat[RID[v] - 1]), --RID[v];
                }
            }
        }
    }

    void bfs() {
        dist.assign(N, -1);
        int ql = 0, qr = 0;
        For(v, N) if (!color[v] && match[v] == -1) que[qr++] = v, dist[v] = 0;
        while (ql < qr) {
            int v = que[ql++];
            For(i, LID[v], MID[v]) {
                auto [to, cost] = dat[i];
                dist[to] = 0;
                int w = match[to];
                if (w != -1 && dist[w] == -1) dist[w] = dist[v] + 1, que[qr++] = w;
            }
        }
    }

    bool dfs(int v) {
        vis[v] = 1;
        For(i, LID[v], MID[v]) {
            auto [to, cost] = dat[i];
            int w = match[to];
            if (w == -1 || (!vis[w] && dist[w] == dist[v] + 1 && dfs(w))) {
                match[to] = v, match[v] = to;
                return true;
            }
        }
        return false;
    }

    vector<int> get_matching_edges() {
        vector<int> match_wt(N, -1);
        vector<int> match_e(N, -1);
        for (auto& e : G.edges) {
            int a = e.frm, b = e.to;
            if (color[a]) swap(a, b);
            if (match[a] == b && chmax(match_wt[a], e.cost)) match_e[a] = e.id;
        }
        vector<int> res;
        For(v, N) if (match_e[v] != -1) res.emplace_back(match_e[v]);
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_RANK_MAXIMAL_BIPARTITE_MATCHING
