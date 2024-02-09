#ifndef AJAY_MANHATTAN_MST
#define AJAY_MANHATTAN_MST

#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// Since it creates a minimum spanning tree, it can also be used especially to find the nearest point from each point.
template <typename T>
Graph<T, 0> manhattan_mst(vector<pair<T, T>>& XY) {
    int N = XY.size();
    vector<tuple<T, int, int>> dat;
    dat.reserve(4 * N);
    vector<int> idx(N);
    iota(idx.begin(), idx.end(), 0);

    For(a, 2) {
        for (auto&& [x, y]: XY) x = -x;
        For(b, 2) {
            for (auto&& [x, y]: XY) swap(x, y);
            sort(idx.begin(), idx.end(), [&](const int& i, const int& j) -> bool {
                return XY[i].first + XY[i].second < XY[j].first + XY[j].second;
            });

            map<T, int> MP;
            for (const int i: idx) {
                auto& [x, y] = XY[i];
                for (auto it = MP.lower_bound(-y); it != MP.end(); it = MP.erase(it)) {
                    const int j = it->se;
                    auto& [xj, yj] = XY[j];
                    const int dx = x - xj;
                    const int dy = y - yj;
                    if (dy > dx) break;
                    dat.eb(dx + dy, i, j);
                }
                MP[-y] = i;
            }
        }
    }

    sort(dat.begin(), dat.end());
    Graph<T, 0> G(N);
    UnionFind uf(N);
    for (auto&& [cost, i, j]: dat) {
        if (uf.merge(i, j)) G.add(i, j, cost);
    }
    G.build();
    return G;
}
} // namespace mitsuha
#endif // AJAY_MANHATTAN_MST