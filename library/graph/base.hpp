#ifndef AJAY_GRAPH
#define AJAY_GRAPH

namespace mitsuha{
template <typename T>
struct Edge {
    int frm, to;
    T cost;
    int id;
};

template <typename T = int, bool directed = false>
struct Graph {
    static constexpr bool is_directed = directed;
    int N, M;
    using cost_type = T;
    using edge_type = Edge<T>;
    vector<edge_type> edges;
    vector<int> indptr;
    vector<edge_type> csr_edges;
    vector<int> vc_deg, vc_indeg, vc_outdeg;
    bool prepared;

    class OutgoingEdges {
    public:
        OutgoingEdges(const Graph* G, int l, int r) : G(G), l(l), r(r) {}

        const edge_type* begin() const {
            if (l == r) { return 0; }
            return &G->csr_edges[l];
        }

        const edge_type* end() const {
            if (l == r) { return 0; }
            return &G->csr_edges[r];
        }

    private:
        const Graph* G;
        int l, r;
    };

    bool is_prepared() { return prepared; }

    Graph() : N(0), M(0), prepared(0) {}
    Graph(int N) : N(N), M(0), prepared(0) {}

    void build(int n) {
        N = n, M = 0;
        prepared = 0;
        edges.clear();
        indptr.clear();
        csr_edges.clear();
        vc_deg.clear();
        vc_indeg.clear();
        vc_outdeg.clear();
    }

    void add(int frm, int to, T cost = 1, int i = -1) {
        assert(!prepared);
        assert(0 <= frm && 0 <= to && to < N);
        if (i == -1) i = M;
        auto e = edge_type({frm, to, cost, i});
        edges.emplace_back(e);
        ++M;
    }

#ifdef FASTIO
    // wt, off
    void read_tree(bool wt = false, int off = 1) { read_graph(N - 1, wt, off); }

    void read_graph(int M, bool wt = false, int off = 1) {
        for (int m = 0; m < M; ++m) {
            int a, b;
            read(a, b);
            a -= off, b -= off;
            if (!wt) {
                add(a, b);
            } else {
                T c;
                read(c);
                add(a, b, c);
            }
        }
        build();
    }
#endif

    void build() {
        assert(!prepared);
        prepared = true;
        indptr.assign(N + 1, 0);
        for (auto&& e: edges) {
            indptr[e.frm + 1]++;
            if (!directed) indptr[e.to + 1]++;
        }
        for (int v = 0; v < N; ++v) { indptr[v + 1] += indptr[v]; }
        auto counter = indptr;
        csr_edges.resize(indptr.back() + 1);
        for (auto&& e: edges) {
            csr_edges[counter[e.frm]++] = e;
            if (!directed)
                csr_edges[counter[e.to]++] = edge_type({e.to, e.frm, e.cost, e.id});
        }
    }

    OutgoingEdges operator[](int v) const {
        assert(prepared);
        return {this, indptr[v], indptr[v + 1]};
    }

    vector<int> deg_array() {
        if (vc_deg.empty()) calc_deg();
        return vc_deg;
    }

    pair<vector<int>, vector<int>> deg_array_inout() {
        if (vc_indeg.empty()) calc_deg_inout();
        return {vc_indeg, vc_outdeg};
    }

    int deg(int v) {
        if (vc_deg.empty()) calc_deg();
        return vc_deg[v];
    }

    int in_deg(int v) {
        if (vc_indeg.empty()) calc_deg_inout();
        return vc_indeg[v];
    }

    int out_deg(int v) {
        if (vc_outdeg.empty()) calc_deg_inout();
        return vc_outdeg[v];
    }

    vector<int> new_idx;
    vector<bool> used_e;

    // vertex V[i] in G become i in the new graph
    // {G, es}
    // The amount of calculation is sum(deg(v)),
    // Be careful as it may be larger than n+m in the new graph
    Graph<T, directed> rearrange(vector<int> V, bool keep_eid = 0) {
        if (len(new_idx) != N) new_idx.assign(N, -1);
        int n = len(V);
        For(i, n) new_idx[V[i]] = i;
        Graph<T, directed> G(n);
        vector<int> history;
        For(i, n) {
            for (auto&& e: (*this)[V[i]]) {
                if (len(used_e) <= e.id) used_e.resize(e.id + 1);
                if (used_e[e.id]) continue;
                int a = e.frm, b = e.to;
                if (new_idx[a] != -1 && new_idx[b] != -1) {
                    history.emplace_back(e.id);
                    used_e[e.id] = 1;
                    int eid = (keep_eid ? e.id : -1);
                    G.add(new_idx[a], new_idx[b], e.cost, eid);
                }
            }
        }
        For(i, n) new_idx[V[i]] = -1;
        for (auto&& eid: history) used_e[eid] = 0;
        G.build();
        return G;
    }

    Graph<T, true> to_directed_tree(int root = -1, bool directed_away_from_root = true) {
        if (root == -1) root = 0;
        assert(!is_directed && prepared && M == N - 1);
        Graph<T, true> G1(N);
        vector<int> par(N, -1);
        auto dfs = [&](auto& dfs, int v) -> void {
            for (auto& e: (*this)[v]) {
                if (e.to == par[v]) continue;
                par[e.to] = v, dfs(dfs, e.to);
            }
        };
        dfs(dfs, root);
        for (auto& e: edges) {
            int a = e.frm, b = e.to;
            if (par[a] == b) swap(a, b);
            assert(par[b] == a);
            if (directed_away_from_root)
                G1.add(a, b, e.cost);
            else
                G1.add(b, a, e.cost);
        }
        G1.build();
        return G1;
    }

private:
    void calc_deg() {
        assert(vc_deg.empty());
        vc_deg.resize(N);
        for (auto&& e: edges) vc_deg[e.frm]++, vc_deg[e.to]++;
    }

    void calc_deg_inout() {
        assert(vc_indeg.empty());
        vc_indeg.resize(N);
        vc_outdeg.resize(N);
        for (auto&& e: edges) { vc_indeg[e.to]++, vc_outdeg[e.frm]++; }
    }
};

template<typename T, bool directed = false>
std::ostream &operator<<(std::ostream &out, const Graph<T, directed> &_G){
    auto G = _G;
    if (not G.prepared) {
        out << "frm to cost id";
        for (auto &&e: G.edges) 
            out << "\n" << e.frm << " " << e.to << " " << e.cost << " " << e.id;
    } else {
        out << "indptr ";
        for(const auto &value : G.indptr) {
            out << value << " ";
        }
        out << "\n";
        out << "frm to cost id";
        for(int v = 0; v < G.N; ++v) 
            for (auto &&e: G[v]) 
            out << "\n" << e.frm << " " << e.to << " " << e.cost << " " << e.id;
    }
    return out;
}
} // namespace mitsuha
#endif // AJAY_GRAPH