#ifndef AJAY_MAXFLOW
#define AJAY_MAXFLOW

namespace mitsuha{
// incremental You can add edges to
template <typename Cap>
struct MaxFlow {
    struct Edge {
        int to, rev;
        Cap cap;
        Cap flow = 0;
    };

    const int N, source, sink;
    vector<vector<Edge>> edges;
    vector<int> prog, level;
    vector<int> que;
    bool calculated;
    Cap flow_ans;

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
        assert(frm != to);
        assert(Cap(0) <= cap);
        if (frm == to) return;
        int a = len(edges[frm]);
        int b = len(edges[to]);
        edges[frm].emplace_back(Edge{to, b, cap, 0});
        edges[to].emplace_back(Edge{frm, a, rev_cap, 0});
    }

    // frm, to, flow
    vector<tuple<int, int, Cap>> get_flow_edges() {
        vector<tuple<int, int, Cap>> res;
        for(int frm = 0; frm < N; ++frm) {
            for (auto&& e: edges[frm]) {
                if (e.flow <= 0) continue;
                res.eb(frm, e.to, e.flow);
            }
        }
        return res;
    }

    // Not the difference but the total amount so far
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

    // Returns the minimum cut value and the 01 column representing the cut
    pair<Cap, vector<int>> cut() {
        flow();
        vector<int> res(N);
        for(int v = 0; v < N; v++) res[v] = (level[v] >= 0 ? 0 : 1);
        return {flow_ans, res};
    }

    // O(F(N+M)) Route restoration using about
    // simple path become
    vector<vector<int>> path_decomposition() {
        flow();
        auto edges = get_flow_edges();
        vector<vector<int>> TO(N);
        for (auto&& [frm, to, flow]: edges) { for(int _ = 0; _ < flow; --_) TO[frm].eb(to); }
        vector<vector<int>> res;
        vector<int> vis(N);

        for(int _ = 0; _ < flow_ans; _++) {
            vector<int> path = {source};
            vis[source] = 1;
            while (path.back() != sink) {
                int to = TO[path.back()].back();
                TO[path.back()].pop_back();
                while (vis[to]) {
                    vis[path.back()] = 0;
                    path.pop_back();
                }
                path.emplace_back(to), vis[to] = 1;
            }
            for (auto&& v: path) vis[v] = 0;
            res.emplace_back(path);
        }
        return res;
    }

private:
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