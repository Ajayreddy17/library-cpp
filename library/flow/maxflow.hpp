#ifndef AJAY_MAXFLOW
#define AJAY_MAXFLOW

#include "library/graph/base.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/maxflow.hpp
// Dinic on a CSR graph. change_capacity(i, c) updates edge i in O((N+M)|delta|).
// Add every edge before the first flow(); use Incremental_MaxFlow to add edges later.

template <typename Cap>
struct MaxFlow {
    struct Edge {
        int to, rev;
        Cap cap;
    };

    vector<Edge> edges;
    vector<int> indptr, level;
    vector<Cap> init_cap;
    vector<tuple<int, int, Cap, Cap>> edge_pool;

    const int N, source, sink;
    bool calculated;
    Cap flow_ans;
    vector<int> pos;

    MaxFlow(int N, int source, int sink)
            : N(N), source(source), sink(sink), calculated(0), flow_ans(0) {}

    void add(int frm, int to, Cap cap, Cap rev_cap = 0) {
        assert(indptr.empty());
        calculated = 0;
        assert(0 <= frm && frm < N);
        assert(0 <= to && to < N);
        assert(Cap(0) <= cap);
        edge_pool.emplace_back(frm, to, cap, rev_cap);
    }

    void build_csr() {
        int M = len(edge_pool);
        indptr.assign(N + 1, 0);
        for (auto [a, b, c, d] : edge_pool) indptr[a + 1]++, indptr[b + 1]++;
        For(i, N) indptr[i + 1] += indptr[i];
        vector<int> prog = indptr;
        edges.resize(2 * M);
        init_cap.resize(2 * M);
        for (auto [a, b, c, d] : edge_pool) {
            int I1 = prog[a]++, I2 = prog[b]++;  // increment before reading b: a == b (self-loop) needs two slots
            edges[I1] = Edge{b, I2, c}, init_cap[I1] = c;
            edges[I2] = Edge{a, I1, d}, init_cap[I2] = d;
            pos.emplace_back(I1);
        }
    }

    Cap flow() {
        if (calculated) return flow_ans;
        calculated = true;
        if (indptr.empty()) build_csr();
        vector<int> prog = indptr;
        level.resize(N);
        vector<int> que(N);
        auto bfs = [&]() -> void {
            fill(level.begin(), level.end(), -1);
            int ql = 0, qr = 0;
            level[source] = 0, que[qr++] = source;
            while (ql < qr) {
                int v = que[ql++];
                For(i, indptr[v], indptr[v + 1]) {
                    Edge& e = edges[i];
                    if (e.cap > 0 && level[e.to] == -1) {
                        level[e.to] = level[v] + 1;
                        que[qr++] = e.to;
                        if (e.to == sink) return;
                    }
                }
            }
        };

        // as in ACL: dfs from the sink, pulling flow back to the source.
        // only vertices on source-sink paths are visited, which makes it faster.
        auto dfs = [&](auto& dfs, int v, Cap lim) -> Cap {
            if (v == source) return lim;
            Cap res = 0;
            int nxt_lv = level[v] - 1;
            for (int& i = prog[v]; i < indptr[v + 1]; ++i) {
                auto& e = edges[i];
                if (level[e.to] != nxt_lv || edges[e.rev].cap == 0) continue;
                Cap a = dfs(dfs, e.to, min(lim - res, edges[e.rev].cap));
                if (a <= 0) continue;
                e.cap += a, edges[e.rev].cap -= a, res += a;
                if (res == lim) return res;
            }
            level[v] = N;
            return res;
        };
        while (1) {
            bfs();
            if (level[sink] == -1) break;
            copy(indptr.begin(), indptr.end(), prog.begin());
            Cap x = dfs(dfs, sink, numeric_limits<Cap>::max() / 2);
            flow_ans += x;
            if (flow_ans >= numeric_limits<Cap>::max() / 2) return numeric_limits<Cap>::max() / 2;
        }
        return flow_ans;
    }

    void change_capacity(int i, Cap after) {
        if (indptr.empty()) build_csr();
        i = pos[i];
        auto& e = edges[i];
        Cap before = init_cap[i];
        Cap f = before - e.cap;
        init_cap[i] = after;
        if (before < after) {
            calculated = (e.cap > 0);
            e.cap += after - before;
            return;
        }
        e.cap = after - f;
        // flow on e now exceeds its capacity: push the excess back
        if (e.cap < 0) flow_push_back(e);
    }

    void flow_push_back(Edge& e0) {
        auto& re0 = edges[e0.rev];
        int a = re0.to;
        int b = e0.to;
        /*
        restore e0.cap >= 0. In a path-cycle decomposition of the flow:
        - cancel cycles through uv: an a->b path in the residual graph (flow_ans unchanged)
        - cancel s->u->v->t paths: t->b and a->s paths in the residual graph
        */

        vector<int> prog(N + 1);
        auto find_path = [&](int s, int t, Cap lim) -> Cap {
            vector<bool> vis(N);
            copy(indptr.begin(), indptr.end(), prog.begin());
            auto dfs = [&](auto& dfs, int v, Cap f) -> Cap {
                if (v == t) return f;
                for (int& i = prog[v]; i < indptr[v + 1]; ++i) {
                    auto& e = edges[i];
                    if (vis[e.to] || e.cap <= Cap(0)) continue;
                    vis[e.to] = 1;
                    Cap a = dfs(dfs, e.to, min(f, e.cap));
                    assert(a >= 0);
                    if (a == Cap(0)) continue;
                    e.cap -= a, edges[e.rev].cap += a;
                    return a;
                }
                return 0;
            };
            return dfs(dfs, s, lim);
        };

        while (e0.cap < 0) {
            Cap x = find_path(a, b, -e0.cap);
            if (x == Cap(0)) break;
            e0.cap += x, re0.cap -= x;
        }
        Cap c = -e0.cap;
        while (c > 0 && a != source) {
            Cap x = find_path(a, source, c);
            assert(x > 0);
            c -= x;
        }
        c = -e0.cap;
        while (c > 0 && b != sink) {
            Cap x = find_path(sink, b, c);
            assert(x > 0);
            c -= x;
        }
        c = -e0.cap;
        e0.cap += c, re0.cap -= c;
        flow_ans -= c;
    }

    // frm, to, flow
    vector<tuple<int, int, Cap>> get_flow_edges() {
        vector<tuple<int, int, Cap>> res;
        For(v, N) {
            For(i, indptr[v], indptr[v + 1]) {
                Edge& e = edges[i];
                Cap f = init_cap[i] - e.cap;
                if (f > 0) res.emplace_back(v, e.to, f);
            }
        }
        return res;
    }

    // residual graph: edge u->v iff residual capacity > 0
    Graph<int, 1> res_graph() {
        flow();
        Graph<int, 1> G(N);
        For(v, N) {
            For(i, indptr[v], indptr[v + 1]) {
                Edge& e = edges[i];
                if (e.cap > 0) {
                    G.add(v, e.to);
                }
            }
        }
        G.build();
        return G;
    }

    // min cut value and the 0/1 side of each vertex
    // side = reachability from source in the residual graph (level may be stale after change_capacity)
    pair<Cap, vector<int>> cut() {
        flow();
        vector<int> res(N, 1), que = {source};
        res[source] = 0;
        For(qi, len(que)) {
            int v = que[qi];
            For(i, indptr[v], indptr[v + 1]) {
                auto& e = edges[i];
                if (e.cap > 0 && res[e.to]) res[e.to] = 0, que.emplace_back(e.to);
            }
        }
        return {flow_ans, res};
    }

    // path decomposition in about O(F(N+M)); every path is simple
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

template <typename Cap>
ostream &operator<<(ostream &out, const MaxFlow<Cap> &Mf) {
        auto mf = Mf;
        if (mf.indptr.empty()) mf.build_csr();
        out << "source " << mf.source << "\n";
        out << "sink " << mf.sink << "\n";
        out << "edges (frm, to, cap, flow)";
        for (int v = 0; v < mf.N; ++v) {
                for (int i = mf.indptr[v]; i < mf.indptr[v + 1]; ++i) {
                        auto &e = mf.edges[i];
                        Cap f = mf.init_cap[i] - e.cap;
                        if (e.cap == 0 && f == 0) continue;
                        out << "\n" << v << " " << e.to << " " << e.cap << " " << f;
                }
        }
        return out;
}
} // namespace mitsuha
#endif // AJAY_MAXFLOW
