#ifndef AJAY_UNIONFIND
#define AJAY_UNIONFIND

namespace mitsuha{
struct UnionFind {
    int n, _classes;
    vector<int> p;
    UnionFind (int n): n(n), _classes(n), p(n, -1){ assert(n >= 0); }
    int classes() const{ return _classes; }
    int root(int u){ return p[u] < 0 ? u : root(p[u]); }
    int operator[](int x) { return root(x); }
    bool share(int a, int b){ return root(a) == root(b); }
    int size(int u){ return -p[root(u)]; }
    
    bool merge(int u, int v){
        u = root(u), v = root(v);
        if(u == v) return false;
        -- _classes;
        if(p[u] > p[v]) swap(u, v);
        p[u] += p[v], p[v] = u;
        return true;
    }
    bool merge(int u, int v, auto act){
        u = root(u), v = root(v);
        if(u == v) return false;
        -- _classes;
        bool swapped = false;
        if(p[u] > p[v]) swap(u, v), swapped = true;
        p[u] += p[v], p[v] = u;
        act(u, v, swapped);
        return true;
    }
    void reset(){
        _classes = n;
        fill(p.begin(), p.end(), -1);
    }
    vector<vector<int>> group_up() {
        vector<vector<int>> g(n);
        for (auto i = 0; i < n; ++i) g[root(i)].push_back(i);
        g.erase(remove_if(g.begin(), g.end(), [&](auto &s) { return s.empty(); }), g.end());
        return g;
    }
};

ostream &operator<<(ostream &out, const UnionFind  &ds){
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