#ifndef AJAY_BLACKBOX_BFS
#define AJAY_BLACKBOX_BFS

namespace mitsuha{
// All edge weights are 1
// [dist, par]
template <typename F1, typename F2>
pair<vector<int>, vector<int>> blackbox_bfs(int N, int s, F1 set_used, F2 find_unused) {
    vector<int> que(N);
    vector<int> dist(N, iinf);
    vector<int> par(N, -1);
    int l = 0, r = 0;
    dist[s] = 0, set_used(s), que[r++] = s;
    while (l < r) {
        int v = que[l++];
        while (1) {
            int to = find_unused(v);
            if (to == -1) break;
            dist[to] = dist[v] + 1, par[to] = v, set_used(to), que[r++] = to;
        }
    }
    return {dist, par};
}
} // namespace mitsuha
#endif // AJAY_BLACKBOX_BFS