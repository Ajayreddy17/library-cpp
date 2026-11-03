#ifndef AJAY_INCREMENTAL_MAXFLOW
#define AJAY_INCREMENTAL_MAXFLOW

#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/incremental_maxflow.hpp
// edges may be added after flow() has been called
template <typename Cap>
struct Incremental_MaxFlow {
    // only the residual graph is stored; flow on an edge = init_cap - cap.
    struct Edge {
        int to;
        Cap cap;
    };

    vector<Edge> edges;
    vector<Cap> init_cap;

    const int N, source, sink;
    vector<int> head, nxt;
    vector<int> prog, level;
    bool calculated;
    Cap flow_ans;

    Incremental_MaxFlow(int N, int source, int sink)
            : N(N),
                source(source),
                sink(sink),
                head(N, -1),
                prog(N, -1),
                level(N, -1),
                calculated(0),
                flow_ans(0) {}

    void add(int frm, int to, Cap cap, Cap rev_cap = 0) {
        calculated = 0;
        assert(0 <= frm && frm < N);
        assert(0 <= to && to < N);
        assert(Cap(0) <= cap);
        if (frm == to) return;
        nxt.emplace_back(head[frm]), head[frm] = len(nxt) - 1, edges.emplace_back(Edge{to, cap});
        nxt.emplace_back(head[to]), head[to] = len(nxt) - 1, edges.emplace_back(Edge{frm, rev_cap});
        init_cap.emplace_back(cap), init_cap.emplace_back(rev_cap);
    }

    Cap flow() {
        if (calculated) return flow_ans;
        calculated = true;
        level.assign(N, -1);  // member: cut() reads it
        vector<int> que(N);
        auto bfs = [&]() -> void {
            fill(level.begin(), level.end(), -1);
            int ql = 0, qr = 0;
            level[source] = 0, que[qr++] = source;
            while (ql < qr) {
                int v = que[ql++];
                for (int i = head[v]; i != -1; i = nxt[i]) {
                    Edge& e = edges[i];
                    if (e.cap > 0 && level[e.to] == -1) {
                        level[e.to] = level[v] + 1;
                        que[qr++] = e.to;
                        if (e.to == sink) return;
                    }
                }
            }
        };
        vector<int> prog = head;
        // as in ACL: push flow from the sink side back to source.
        // only vertices on source-sink paths are visited, which makes it faster.
        auto dfs = [&](auto& dfs, int v, Cap lim) -> Cap {
            if (v == source) return lim;
            Cap res = 0;
            int nxt_lv = level[v] - 1;
            for (int& i = prog[v]; i != -1; i = nxt[i]) {
                auto& e = edges[i];
                if (level[e.to] != nxt_lv || edges[i ^ 1].cap == 0) continue;
                Cap a = dfs(dfs, e.to, min(lim - res, edges[i ^ 1].cap));
                e.cap += a, edges[i ^ 1].cap -= a, res += a;
                if (res == lim) return res;
            }
            level[v] = N;
            return res;
        };
        while (1) {
            bfs();
            if (level[sink] == -1) break;
            copy(head.begin(), head.end(), prog.begin());
            Cap x = dfs(dfs, sink, numeric_limits<Cap>::max() / 2);
            flow_ans += x;
            if (flow_ans >= numeric_limits<Cap>::max() / 2) return numeric_limits<Cap>::max() / 2;
        }
        return flow_ans;
    }  // frm, to, flow
    vector<tuple<int, int, Cap>> get_flow_edges() {
        vector<tuple<int, int, Cap>> res;
        For(v, N) {
            for (int i = head[v]; i != -1; i = nxt[i]) {
                Edge& e = edges[i];
                Cap f = init_cap[i] - e.cap;
                if (f > 0) res.emplace_back(v, e.to, f);
            }
        }
        return res;
    }

    // min cut value and the 0/1 side of each vertex
    pair<Cap, vector<int>> cut() {
        flow();
        vector<int> res(N);
        For(v, N) res[v] = (level[v] >= 0 ? 0 : 1);
        return {flow_ans, res};
    }

    // path decomposition in about O(F(N+M))
    // every path is simple
    vector<vector<int>> path_decomposition() {
        flow();
        auto edges = get_flow_edges();
        vector<vector<int>> TO(N);
        for (auto&& [frm, to, flow] : edges) {
            For(flow) TO[frm].emplace_back(to);
        }
        vector<vector<int>> res;
        vector<int> vis(N);

        For(flow_ans) {
            vector<int> path = {source};
            vis[source] = 1;
            while (path.back() != sink) {
                int to = POP(TO[path.back()]);
                while (vis[to]) {
                    vis[POP(path)] = 0;
                }
                path.emplace_back(to), vis[to] = 1;
            }
            for (auto&& v : path) vis[v] = 0;
            res.emplace_back(path);
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_INCREMENTAL_MAXFLOW
