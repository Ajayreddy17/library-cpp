#ifndef AJAY_GRAPH_BLACKBOX_TOPOSORT
#define AJAY_GRAPH_BLACKBOX_TOPOSORT


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/blackbox/toposort.hpp
// set_used(v): mark v as used
// find_unused(v): find a successor of v; return -1 if none
// https://codeforces.com/contest/1158/problem/C
template <typename F0, typename F1, typename F2>
vector<int> blackbox_toposort(int N, F0 init, F1 set_used, F2 find_unused,
                                                    bool check = true) {
    init();
    vector<int> V;
    vector<bool> done(N);
    auto dfs = [&](auto& dfs, int v) -> void {
        set_used(v);
        done[v] = 1;
        while (1) {
            int to = find_unused(v);
            if (to == -1) break;
            dfs(dfs, to);
        }
        V.emplace_back(v);
    };
    For(v, N) if (!done[v]) dfs(dfs, v);
    reverse(V.begin(), V.end());
    if (check) {
        init();
        Frr(i, N) {
            int v = V[i];
            if (find_unused(v) != -1) {
                return {};
            }
            set_used(v);
        }
    }
    return V;
}
} // namespace mitsuha
#endif // AJAY_GRAPH_BLACKBOX_TOPOSORT
