#ifndef AJAY_UNIONFIND
#define AJAY_UNIONFIND

namespace mitsuha{
struct UnionFind{
    int n, n_comp;
    vector<int> p;
    vector<list<int>> group;
    UnionFind(int n = 0) { build(n); }
    void build(int m) {
        Assert(m >= 0);
        n = m, n_comp = m;
        p.assign(n, -1);
        group.assign(n, {});
        for(auto i = 0; i < n; ++ i) group[i] = {i};
    }
    void reset() { build(n); }
    int make_set(){
        p.push_back(-1);
        group.push_back(list<int>{n});
        ++ n_comp;
        return n ++;
    }
    int add_vertex() { return make_set(); }
    int root(int u){
        Assert(0 <= u && u < n);
        return p[u] < 0 ? u : p[u] = root(p[u]);
    }
    int operator[](int u) { return root(u); }
    bool same(int u, int v){
        Assert(0 <= min(u, v) && max(u, v) < n);
        return root(u) == root(v);
    }
    int size(int u){
        Assert(0 <= u && u < n);
        return -p[root(u)];
    }
    bool merge(int u, int v){
        Assert(0 <= min(u, v) && max(u, v) < n);
        u = root(u), v = root(v);
        if(u == v) return false;
        -- n_comp;
        if(p[u] > p[v]) swap(u, v);
        p[u] += p[v], p[v] = u;
        group[u].splice(group[u].end(), group[v]);
        return true;
    }
    int group_count() const{
        return n_comp;
    }
    const list<int> &group_of(int u){
        Assert(0 <= u && u < n);
        return group[root(u)];
    }
    vector<vector<int>> group_up(){
        vector<vector<int>> g(n);
        for(auto i = 0; i < n; ++ i) g[root(i)].push_back(i);
        g.erase(remove_if(g.begin(), g.end(), [&](auto &s){ return s.empty(); }), g.end());
        return g;
    }
    vector<int> get_all() {
        vector<int> A(n);
        for(auto i = 0; i < n; ++ i) A[i] = root(i);
        return A;
    }
    void clear(){
        n_comp = n;
        fill(p.begin(), p.end(), -1);
        for(auto i = 0; i < n; ++ i) group[i] = {i};
    }
};

ostream &operator<<(ostream &out, const UnionFind &ds){
    auto dsu = ds;
    auto gs = dsu.group_up();
    out << "{";
    if(!gs.empty()) for(auto i = 0; i < (int)gs.size(); ++ i){
            out << "{";
            for(auto j = 0; j < (int)gs[i].size(); ++ j){
                out << gs[i][j];
                if(j + 1 < (int)gs[i].size()) out << ", ";
            }
            out << "}";
            if(i + 1 < (int)gs.size()) out << ", ";
        }
    return out << "}";
}
} // namespace mitsuha
#endif // AJAY_UNIONFIND