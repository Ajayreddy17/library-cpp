#ifndef AJAY_TWO_SAT
#define AJAY_TWO_SAT

namespace mitsuha{
struct Two_Sat{
    vector<int> value;
    Two_Sat(int n = 0): n(n), adj(n << 1){ }
    int add_variable(){
        adj.emplace_back();
        adj.emplace_back();
        return n++;
    }
    void either(int u, int v){
        u = max(2 * u, -1 - 2 * u);
        v = max(2 * v, -1 - 2 * v);
        adj[u].push_back(v ^ 1);
        adj[v].push_back(u ^ 1);
    }
    void implies(int u, int v){
        either(~u, v);
    }
    void equals(int u, int v){
        either(~u, v), either(u, ~v);
    }
    void differs(int u, int v){
        either(u, v), either(~u, ~v);
    }
    void set_value(int u, bool x = true){
        x ? either(u, u) : either(~u, ~u);
    }
    void at_most_one(const vector<int> &arr){
        if((int)arr.size() <= 1) return;
        int cur = ~arr[0];
        for(auto u = 2; u < (int)arr.size(); ++ u){
            int next = add_variable();
            either(cur, ~arr[u]), either(cur, next), either(~arr[u], next);
            cur = ~next;
        }
        either(cur, ~arr[1]);
    }
    int time, comp_cnt;
    int dfs(int u){
        int low = val[u] = ++ time, v;
        z.push_back(u);
        for(auto v: adj[u]) if(!~comp[v]) low = min(low, val[v] ?: dfs(v));
        ++ time;
        if(low == val[u]){
            do{
                v = z.back();
                z.pop_back();
                comp[v] = comp_cnt;
                if(value[v >> 1] == -1) value[v >> 1] = v & 1;
            }while(v != u);
            comp_cnt ++;
        }
        return val[u] = low;
    }
    // O(n)
    bool solve(){
        value.assign(n, -1);
        val.assign(2 * n, 0);
        comp.assign(2 * n, -1);
        time = comp_cnt = 0;
        for(auto u = 0; u < n << 1; ++ u) if(!~comp[u]) dfs(u);
        for(auto u = 0; u < n; ++ u) if(comp[u << 1] == comp[u << 1 ^ 1]) return false;
        return true;
    }
    // Enumerate solutions while act_while() returns true.
    // O(n^2 + (m + n * (# of solutions found))*S/w)
    template<size_t S>
    bool enumerate_solutions(auto act_while){
        assert(2 * n <= S);
        if(!solve()) return false;
        fill(value.begin(), value.end(), -1);
        bitset<S> has_value;
        vector<vector<int>> has(comp_cnt);
        vector<bitset<S>> reachable(n << 1);
        for(auto u = 0; u < n << 1; ++ u) has[comp[u]].push_back(u);
        vector<int> vis(comp_cnt, -1);
        for(auto t = 0; t < comp_cnt; ++ t){
            int u = has[t][0];
            vis[t] = t;
            for(auto v: has[t]){
                reachable[u].set(v);
                for(auto w: adj[v]) if(vis[comp[w]] != t){
                    vis[comp[w]] = t;
                    reachable[u] |= reachable[w];
                }
            }
            for(auto v: has[t]) reachable[v] = reachable[u];
        }
        for(auto u = 0; u < n << 1; ++ u){
            if(!reachable[u][u ^ 1]) continue;
            has_value[u] = has_value[u ^ 1] = true;
            value[u >> 1] = ~u & 1;
            for(auto v = 0; v < n << 1; ++ v) if(reachable[u ^ 1][v]){
                has_value[v] = has_value[v ^ 1] = true;
                value[v >> 1] = v & 1;
            }
        }
        vector<bitset<S>> delta(comp_cnt);
        auto dfs = [&](auto self, int t)->bool{
            if(!~t) return act_while(value);
            int u = has[t][0];
            if(~value[u >> 1]) return self(self, t - 1);
            for(auto v: has[t]){
                has_value[v] = has_value[v ^ 1] = true;
                value[v >> 1] = ~v & 1;
            }
            if(!self(self, t - 1)) return false;
            for(auto v: has[t]) value[v >> 1] = v & 1;
            delta[t] = reachable[u] & ~has_value;
            for(auto v = delta[t]._Find_first(); v != S; v = delta[t]._Find_next(v)){
                has_value[v] = has_value[v ^ 1] = true;
                value[v >> 1] = v & 1;
            }
            if(!self(self, t - 1)) return false;
            for(auto v: has[t]){
                has_value[v] = has_value[v ^ 1] = false;
                value[v >> 1] = -1;
            }
            for(auto v = delta[t]._Find_first(); v != S; v = delta[t]._Find_next(v)){
                has_value[v] = has_value[v ^ 1] = false;
                value[v >> 1] = -1;
            }
            return true;
        };
        dfs(dfs, comp_cnt - 1);
        return true;
    }
private:
    int n;
    vector<vector<int>> adj;
    vector<int> val, comp, z;
};
} // namespace mitsuha
#endif // AJAY_TWO_SAT