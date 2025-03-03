#ifndef AJAY_MAXFLOW
#define AJAY_MAXFLOW

namespace mitsuha{
// Incremental MaxFlow
// Edge Cap can be changed, 
// When the capacity to change is F, update in O((N+M)|F|) time
template <typename Cap>
struct MaxFlow {
    struct Edge {
        int to, rev;
        Cap cap; // Indicates remaining capacity. Therefore, cap+flow is a constant.
        Cap flow = 0;
    };

    const int N, source, sink;
    vector<vector<Edge>> edges;
    vector<pair<int, int>> pos;
    vector<int> prog, level;
    vector<int> que;
    bool calculated;

    MaxFlow(int N, int source, int sink)
            : N(N),
              source(source),
              sink(sink),
              edges(N),
              calculated(0),
              flow_ans(0) {}

    void add(int frm, int to, Cap cap, Cap rev_cap = 0) {
        calculated = 0;
        assert(0 <= frm && frm < N);
        assert(0 <= to && to < N);
        assert(Cap(0) <= cap);
        int a = len(edges[frm]);
        int b = (frm == to ? a + 1 : len(edges[to]));
        pos.emplace_back(frm, a);
        edges[frm].emplace_back(Edge{to, b, cap, 0});
        edges[to].emplace_back(Edge{frm, a, rev_cap, 0});
    }

    void change_capacity(int i, Cap after) {
        auto [frm, idx] = pos[i];
        auto& e = edges[frm][idx];
        Cap before = e.cap + e.flow;
        if (before < after) {
            calculated = (e.cap > 0);
            e.cap += after - before;
            return;
        }
        e.cap = after - e.flow;
        if (e.cap < 0) flow_push_back(e);
    }

    void flow_push_back(Edge& e0) {
        auto& re0 = edges[e0.to][e0.rev];
        int a = re0.to;
        int b = e0.to;

        auto find_path = [&](int s, int t, Cap lim) -> Cap {
            vector<bool> vis(N);
            prog.assign(N, 0);
            auto dfs = [&](auto& dfs, int v, Cap f) -> Cap {
                if (v == t) return f;
                for (int& i = prog[v]; i < len(edges[v]); ++i) {
                    auto& e = edges[v][i];
                    if (vis[e.to] || e.cap <= Cap(0)) continue;
                    vis[e.to] = 1;
                    Cap a = dfs(dfs, e.to, min(f, e.cap));
                    assert(a >= 0);
                    if (a == Cap(0)) continue;
                    e.cap -= a, e.flow += a;
                    edges[e.to][e.rev].cap += a, edges[e.to][e.rev].flow -= a;
                    return a;
                }
                return 0;
            };
            return dfs(dfs, s, lim);
        };

        while (e0.cap < 0) {
            Cap x = find_path(a, b, -e0.cap);
            if (x == Cap(0)) break;
            e0.cap += x, e0.flow -= x;
            re0.cap -= x, re0.flow += x;
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
        e0.cap += c, e0.flow -= c;
        re0.cap -= c, re0.flow += c;
        flow_ans -= c;
    }

    // frm, to, flow
    vector<tuple<int, int, Cap>> get_flow_edges() {
        vector<tuple<int, int, Cap>> res;
        For(frm, N) {
            for (auto&& e: edges[frm]) {
                if (e.flow <= 0) continue;
                res.emplace_back(frm, e.to, e.flow);
            }
        }
        return res;
    }

    Cap flow() {
        if (calculated) return flow_ans;
        calculated = true;
        while (set_level()) {
            prog.assign(N, 0);
            while (1) {
                Cap x = flow_dfs(source, numeric_limits<Cap>::max() / 2);
                if (x == 0) break;
                flow_ans += x;
                chmin(flow_ans, numeric_limits<Cap>::max() / 2);
                if (flow_ans == numeric_limits<Cap>::max() / 2) return flow_ans;
            }
        }
        return flow_ans;
    }

    // Returns the minimum cut value and the 01 side of cut
    pair<Cap, vector<int>> cut() {
        flow();
        vector<int> res(N);
        For(v, N) res[v] = (level[v] >= 0 ? 0 : 1);
        return {flow_ans, res};
    }


    // Restore the route using about O(F(N+M))
    // becomes a simple path
    vector<vector<int>> path_decomposition() {
        flow();
        auto edges = get_flow_edges();
        vector<vector<int>> TO(N);
        for (auto&& [frm, to, flow]: edges) { For(flow) TO[frm].emplace_back(to); }
        vector<vector<int>> res;
        vector<int> vis(N);

        For(flow_ans) {
            vector<int> path = {source};
            vis[source] = 1;
            while (path.back() != sink) {
                int to = TO[path.back()].back(); TO[path.back()].pop_back();
                while (vis[to]) {
                    vis[path.back()] = 0; path.pop_back();
                }
                path.emplace_back(to), vis[to] = 1;
            }
            for (auto&& v: path) vis[v] = 0;
            res.emplace_back(path);
        }
        return res;
    }

private:
    Cap flow_ans;

    bool set_level() {
        que.resize(N);
        level.assign(N, -1);
        level[source] = 0;
        int l = 0, r = 0;
        que[r++] = source;
        while (l < r) {
            int v = que[l++];
            for (auto&& e: edges[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    if (e.to == sink) return true;
                    que[r++] = e.to;
                }
            }
        }
        return false;
    }

    Cap flow_dfs(int v, Cap lim) {
        if (v == sink) return lim;
        Cap res = 0;
        for (int& i = prog[v]; i < len(edges[v]); ++i) {
            auto& e = edges[v][i];
            if (e.cap > 0 && level[e.to] == level[v] + 1) {
                Cap a = flow_dfs(e.to, min(lim, e.cap));
                if (a > 0) {
                    e.cap -= a, e.flow += a;
                    edges[e.to][e.rev].cap += a, edges[e.to][e.rev].flow -= a;
                    res += a;
                    lim -= a;
                    if (lim == 0) break;
                }
            }
        }
        return res;
    }
};

template <typename Cap>
ostream &operator<<(ostream &out, const MaxFlow<Cap> &Mf){
    auto mf = Mf;
    out << "source " << mf.source << "\n";
    out << "sink " << mf.sink << "\n";
    out << "edges (frm, to, cap, flow)";
    for(int v = 0; v < mf.N; ++v) {
        for (auto& e: mf.edges[v]) {
            if (e.cap == 0 && e.flow == 0) continue;
            out << "\n" << v << " " << e.to << " " << e.cap << " " << e.flow;
        }
    }
    return out;
}
} // namespace mitsuha
#endif // AJAY_MAXFLOW