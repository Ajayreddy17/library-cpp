#ifndef AJAY_UNIONFIND
#define AJAY_UNIONFIND

namespace mitsuha{
struct UnionFind{
    int n, _group_count;
    vector<int> p;
    vector<list<int>> group;
    UnionFind(){ }
    UnionFind(int n): n(n), _group_count(n), p(n, -1), group(n){
        Assert(n >= 0);
        for(auto i = 0; i < n; ++ i) group[i] = {i};
    }
    int make_set(){
		p.push_back(-1);
		group.push_back(list<int>{n});
		++ _group_count;
		return n ++;
	}
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
        -- _group_count;
        if(p[u] > p[v]) swap(u, v);
        p[u] += p[v], p[v] = u;
        group[u].splice(group[u].end(), group[v]);
        return true;
    }
    int group_count() const{
        return _group_count;
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
    void clear(){
        _group_count = n;
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