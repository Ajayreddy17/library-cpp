#ifndef AJAY_MIN_COST_FLOW
#define AJAY_MIN_COST_FLOW

namespace mitsuha{
namespace internal {
template <class E>
struct csr {
    vector<int> start;
    vector<E> elist;
    explicit csr(int n, const vector<pair<int, E>>& edges): start(n + 1), elist(edges.size()) {
        for (auto e: edges) { start[e.first + 1]++; }
        for (int i = 1; i <= n; i++) { start[i] += start[i - 1]; }
        auto counter = start;
        for (auto e: edges) { elist[counter[e.first]++] = e.second; }
    }
};

template <class T>
struct simple_queue {
    vector<T> payload;
    int pos = 0;
    void reserve(int n) { payload.reserve(n); }
    int size() const { return int(payload.size()) - pos; }
    bool empty() const { return pos == int(payload.size()); }
    void push(const T& t) { payload.push_back(t); }
    T& front() { return payload[pos]; }
    void clear() {
        payload.clear();
        pos = 0;
    }
    void pop() { pos++; }
};
}

/*
Slightly modified version of atcoder library
If DAG = true, negative side OK (1st shortest path is done with dp)
However, it is assumed that the vertex numbers have been toposorted.
*/
template <class Cap = int, class Cost = long long, bool DAG = false>
struct Min_Cost_Flow {
public:
    Min_Cost_Flow() {}
    explicit Min_Cost_Flow(int n, int source, int sink) : n(n), source(source), sink(sink) {
        assert(0 <= source && source < n);
        assert(0 <= sink && sink < n);
        assert(source != sink);
    }

    // frm, to, cap, cost
    int add(int frm, int to, Cap cap, Cost cost) {
        assert(0 <= frm && frm < n);
        assert(0 <= to && to < n);
        assert(0 <= cap);
        assert(DAG || 0 <= cost);
        if (DAG) assert(frm < to);
        int m = int(_edges.size());
        _edges.push_back({frm, to, cap, 0, cost});
        return m;
    }

    struct edge {
        int frm, to;
        Cap cap, flow;
        Cost cost;
    };

    edge get_edge(int i) {
        int m = int(_edges.size());
        assert(0 <= i && i < m);
        return _edges[i];
    }
    vector<edge> edges() { return _edges; }

    pair<Cap, Cost> flow() { return flow(numeric_limits<Cap>::max() / 2); }
    pair<Cap, Cost> flow(Cap flow_limit) { return slope(flow_limit).back(); }
    vector<pair<Cap, Cost>> slope() { return slope(numeric_limits<Cap>::max() / 2); }
    vector<pair<Cap, Cost>> slope(Cap flow_limit) {
        int m = int(_edges.size());
        vector<int> edge_idx(m);

        auto g = [&]() {
            vector<int> degree(n), redge_idx(m);
            vector<pair<int, _edge>> elist;
            elist.reserve(2 * m);
            for (int i = 0; i < m; i++) {
                auto e = _edges[i];
                edge_idx[i] = degree[e.frm]++;
                redge_idx[i] = degree[e.to]++;
                elist.push_back({e.frm, {e.to, -1, e.cap - e.flow, e.cost}});
                elist.push_back({e.to, {e.frm, -1, e.flow, -e.cost}});
            }
            auto _g = internal::csr<_edge>(n, elist);
            for (int i = 0; i < m; i++) {
                auto e = _edges[i];
                edge_idx[i] += _g.start[e.frm];
                redge_idx[i] += _g.start[e.to];
                _g.elist[edge_idx[i]].rev = redge_idx[i];
                _g.elist[redge_idx[i]].rev = edge_idx[i];
            }
            return _g;
        }();

        auto result = slope(g, flow_limit);

        for (int i = 0; i < m; i++) {
            auto e = g.elist[edge_idx[i]];
            _edges[i].flow = _edges[i].cap - e.cap;
        }

        return result;
    }

    // O(F(N+M)) Route restoration using about
    vector<vector<int>> path_decomposition() {
        vector<vector<int>> TO(n);
        for (auto&& e: edges()) { for(int _ = 0; _ < e.flow; ++_) TO[e.frm].eb(e.to); }
        vector<vector<int>> res;
        vector<int> vis(n);

        while (!TO[source].empty()) {
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


    int n, source, sink;
    vector<edge> _edges;
    
private:
    // inside edge
    struct _edge {
        int to, rev;
        Cap cap;
        Cost cost;
    };

    vector<pair<Cap, Cost>> slope(internal::csr<_edge>& g, Cap flow_limit) {
        if (DAG) assert(source == 0 && sink == n - 1);
        vector<pair<Cost, Cost>> dual_dist(n);
        vector<int> prev_e(n);
        vector<bool> vis(n);
        struct Q {
            Cost key;
            int to;
            bool operator<(Q r) const { return key > r.key; }
        };
        vector<int> que_min;
        vector<Q> que;
        auto dual_ref = [&]() {
            for (int i = 0; i < n; i++) { dual_dist[i].second = numeric_limits<Cost>::max() / 2; }
            fill(vis.begin(), vis.end(), false);
            que_min.clear();
            que.clear();

            // que[0..heap_r) was heapified
            size_t heap_r = 0;

            dual_dist[source].second = 0;
            que_min.push_back(source);
            while (!que_min.empty() || !que.empty()) {
                int v;
                if (!que_min.empty()) {
                    v = que_min.back();
                    que_min.pop_back();
                } else {
                    while (heap_r < que.size()) {
                        heap_r++;
                        push_heap(que.begin(), que.begin() + heap_r);
                    }
                    v = que.front().to;
                    pop_heap(que.begin(), que.end());
                    que.pop_back();
                    heap_r--;
                }
                if (vis[v]) continue;
                vis[v] = true;
                if (v == sink) break;
                Cost dual_v = dual_dist[v].first, dist_v = dual_dist[v].second;
                for (int i = g.start[v]; i < g.start[v + 1]; i++) {
                    auto e = g.elist[i];
                    if (!e.cap) continue;
                    Cost cost = e.cost - dual_dist[e.to].first + dual_v;
                    if (dual_dist[e.to].second > dist_v + cost) {
                        Cost dist_to = dist_v + cost;
                        dual_dist[e.to].second = dist_to;
                        prev_e[e.to] = e.rev;
                        if (dist_to == dist_v) {
                            que_min.push_back(e.to);
                        } else {
                            que.push_back(Q{dist_to, e.to});
                        }
                    }
                }
            }
            if (!vis[sink]) { return false; }

            for (int v = 0; v < n; v++) {
                if (!vis[v]) continue;
                dual_dist[v].first -= dual_dist[sink].second - dual_dist[v].second;
            }
            return true;
        };

        auto dual_ref_dag = [&]() {
            for(int i = 0; i < n; ++i) dual_dist[i].second = numeric_limits<Cost>::max() / 2;
            dual_dist[source].second = 0;
            fill(vis.begin(), vis.end(), false);
            vis[0] = true;

            for(int v = 0; v < n; ++v) {
                if (!vis[v]) continue;
                Cost dual_v = dual_dist[v].first, dist_v = dual_dist[v].second;
                for (int i = g.start[v]; i < g.start[v + 1]; i++) {
                    auto e = g.elist[i];
                    if (!e.cap) continue;
                    Cost cost = e.cost - dual_dist[e.to].first + dual_v;
                    if (dual_dist[e.to].second > dist_v + cost) {
                        vis[e.to] = true;
                        Cost dist_to = dist_v + cost;
                        dual_dist[e.to].second = dist_to;
                        prev_e[e.to] = e.rev;
                    }
                }
            }
            if (!vis[sink]) { return false; }

            for (int v = 0; v < n; v++) {
                if (!vis[v]) continue;
                dual_dist[v].first -= dual_dist[sink].second - dual_dist[v].second;
            }
            return true;
        };

        Cap flow = 0;
        Cost cost = 0, prev_cost_per_flow = -1;
        vector<pair<Cap, Cost>> result = {{Cap(0), Cost(0)}};
        while (flow < flow_limit) {
            if (DAG && flow == 0) {
                if (!dual_ref_dag()) break;
            } else {
                if (!dual_ref()) break;
            }
            Cap c = flow_limit - flow;
            for (int v = sink; v != source; v = g.elist[prev_e[v]].to) {
                c = min(c, g.elist[g.elist[prev_e[v]].rev].cap);
            }
            for (int v = sink; v != source; v = g.elist[prev_e[v]].to) {
                auto& e = g.elist[prev_e[v]];
                e.cap += c;
                g.elist[e.rev].cap -= c;
            }
            Cost d = -dual_dist[source].first;
            flow += c;
            cost += c * d;
            if (prev_cost_per_flow == d) { result.pop_back(); }
            result.push_back({flow, cost});
            prev_cost_per_flow = d;
        }
        return result;
    }
};

template <class Cap = int, class Cost = long long, bool DAG = false>
ostream &operator<<(ostream &out, const Min_Cost_Flow<Cap, Cost, DAG> &Mcf){
    auto mcf = Mcf;
    out << "flow graph\n";
    out << "frm, to, cap, cost";
    for (auto&& [frm, to, cap, flow, cost]: mcf._edges) {
        out << "\n" << frm << " " << to << " " << cap << " " << cost;
    }
    return out;
}
} // namespace mitsuha
#endif // AJAY_MIN_COST_FLOW