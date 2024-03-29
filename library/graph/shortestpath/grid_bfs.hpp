#ifndef AJAY_GRID_BFS
#define AJAY_GRID_BFS

namespace mitsuha{
// For example, walls = "#" or walls = {-1}.
template <typename STRING>
vector<vector<int>> grid_bfs(vector<STRING>& G, vector<pair<int, int>> &src, STRING walls, int dmax = 4) {
    assert(dmax == 4 or dmax == 8);
    int H = (int) G.size();
    int W = (int) G[0].size();
    auto isin = [&](int x, int y) -> bool {
        if (x < 0 or H <= x) return false;
        if (y < 0 or W <= y) return false;
        return count(walls.begin(), walls.end(), G[x][y]) == 0;
    };
    int dx[] = {1, 0, -1, 0, 1, -1, -1, 1};
    int dy[] = {0, 1, 0, -1, 1, 1, -1, -1};
    vector<vector<int>> dist(H, vector<int>(W, numeric_limits<int>::max() / 2));
    deque<pair<int, int>> que;
    auto add = [&](int x, int y, int d) -> void {
        if (!isin(x, y)) return;
        if (d >= dist[x][y] ? false : (dist[x][y] = d, true)) que.emplace_back(x, y);
    };
    for (auto &&[sx, sy]: src) {
        add(sx, sy, 0);
    }

    while (not que.empty()) {
        auto [x, y] = que.front();
        que.pop_front();
        for(int d = 0; d < dmax; d ++) {
            int nx = x + dx[d], ny = y + dy[d];
            add(nx, ny, dist[x][y] + 1);
        }
    }
    return dist;
}

// For example, walls = "#" or walls = {-1}.
template <typename STRING>
vector<vector<int>> grid_bfs(vector<STRING>& G, int sx, int sy, STRING walls, int dmax = 4) {
    vector<pair<int, int>> src; src.emplace_back(sx, sy);
    return grid_bfs(G, src, walls, dmax);
}
} // namespace mitsuha
#endif // AJAY_GRID_BFS