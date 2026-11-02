#ifndef AJAY_GRAPH_BLACKBOX_SCC
#define AJAY_GRAPH_BLACKBOX_SCC


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/blackbox/scc.hpp

// dfs on G and on its reverse graph
// the topological order is correct, but the edges seen while searching do not give scc_dag
// set_used(int v, bool rev)
// find_used(int v, bool rev)
// find is called after set. There is a rev=false phase and a rev=true phase.
template <typename F1, typename F2>
pair<int, vector<int>> blackbox_scc(int N, F1 set_used, F2 find_unused) {
    vector<int> ord(N);
    {
        int nxt = N;
        vector<bool> vis(N);
        auto dfs = [&](auto& dfs, int v) -> void {
            assert(v < N && !vis[v]);
            vis[v] = 1, set_used(v, false);
            while (1) {
                int to = find_unused(v, false);
                assert(to < N);
                if (to == -1) break;
                dfs(dfs, to);
            }
            ord[--nxt] = v;
        };
        For(v, N) if (!vis[v]) dfs(dfs, v);
    }
    vector<int> comp(N);
    int nc = 0;
    vector<bool> vis(N);
    auto dfs = [&](auto& dfs, int v) -> void {
        vis[v] = 1, comp[v] = nc, set_used(v, true);
        while (1) {
            int to = find_unused(v, true);
            assert(to < N);
            if (to == -1) break;
            dfs(dfs, to);
        }
    };
    for (auto&& v : ord) {
        if (!vis[v]) dfs(dfs, v), ++nc;
    }
    return {nc, comp};
}
} // namespace mitsuha
#endif // AJAY_GRAPH_BLACKBOX_SCC
