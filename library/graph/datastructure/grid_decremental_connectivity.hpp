#ifndef AJAY_GRID_DECREMENTAL_CONNECTIVITY
#define AJAY_GRID_DECREMENTAL_CONNECTIVITY

#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/grid_decremental_connectivity.hpp

// https://codeforces.com/contest/1864/problem/I
// edge deletion / connected components of grid cells (4-neighbourhood)
struct Grid_Decremental_Connectivity {
    int H, W, n_comp;
    vector<vector<int>> A;  // component id of each cell
    vector<int> sz;
    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    // already deleted?
    vector<vector<bool>> TATE;
    vector<vector<bool>> YOKO;

    UnionFind dual_uf;
    int bfs_time = 0;
    vector<vector<int>> vis;

    Grid_Decremental_Connectivity(int H, int W) : H(H), W(W), n_comp(1) {
        A = vector<vector<int>>(H, vector<int>(W, 0));
        vis = vector<vector<int>>(H, vector<int>(W, 0));
        TATE = vector<vector<bool>>(H, vector<bool>(W + 1, false));
        YOKO = vector<vector<bool>>(H + 1, vector<bool>(W, false));
        dual_uf.build((H + 1) * (W + 1));
        sz.emplace_back(H * W);
        // cut grid border
        For(x, H) {
            dual_uf.merge(dual_idx(x, 0), dual_idx(x + 1, 0));
            dual_uf.merge(dual_idx(x, W), dual_idx(x + 1, W));
            TATE[x][0] = TATE[x][W] = true;
        }
        For(y, W) {
            dual_uf.merge(dual_idx(0, y), dual_idx(0, y + 1));
            dual_uf.merge(dual_idx(H, y), dual_idx(H, y + 1));
            YOKO[0][y] = YOKO[H][y] = true;
        }
    }

    int dual_idx(int x, int y) const { return (W + 1) * x + y; }
    int get(int x, int y) const { return A[x][y]; }
    bool isin(int x, int y) const { return 0 <= x && x < H && 0 <= y && y < W; }

    bool can_go(int x1, int y1, int x2, int y2) {
        if (!isin(x1, y1) || !isin(x2, y2)) return false;
        if (x1 + y1 > x2 + y2) swap(x1, x2), swap(y1, y2);
        if (x1 == x2 && y2 == y1 + 1) {
            return !TATE[x1][y1 + 1];
        }
        elif (y1 == y2 && x2 == x1 + 1) { return !YOKO[x1 + 1][y1]; }
        assert(false);
        return false;
    }

    void cut(int x1, int y1, int x2, int y2) {
        assert(isin(x1, y1) && isin(x2, y2) && A[x1][y1] == A[x2][y2]);
        if (x1 + y1 > x2 + y2) swap(x1, x2), swap(y1, y2);
        int a1, b1, a2, b2;
        if (x1 == x2 && y2 == y1 + 1) {
            a1 = x1, b1 = y1 + 1, a2 = x1 + 1, b2 = y1 + 1;
            assert(!TATE[a1][b1]);
            TATE[a1][b1] = true;
        }
        elif (y1 == y2 && x2 == x1 + 1) {
            a1 = x1 + 1, b1 = y1, a2 = x1 + 1, b2 = y1 + 1;
            assert(!YOKO[a1][b1]);
            YOKO[a1][b1] = true;
        }
        else {
            assert(false);
        }
        if (dual_uf.merge(dual_idx(a1, b1), dual_idx(a2, b2))) {
            // keep connectivity
            return;
        }

        // parallel bfs
        bfs_time++;
        vector<pair<int, int>> Q1, Q2;
        Q1.emplace_back(x1, y1), Q2.emplace_back(x2, y2);
        vis[x1][y1] = vis[x2][y2] = bfs_time;
        int p1 = 0, p2 = 0;
        while (1) {
            if (p1 == len(Q1)) {
                p2 = infty<int>;
                break;
            }
            if (p2 == len(Q2)) {
                p1 = infty<int>;
                break;
            }
            For(2) {
                swap(p1, p2), swap(Q1, Q2);
                auto [x, y] = Q1[p1++];
                For(d, 4) {
                    int xx = x + dx[d], yy = y + dy[d];
                    if (isin(xx, yy) && vis[xx][yy] < bfs_time && can_go(x, y, xx, yy)) {
                        assert(A[x][y] == A[xx][yy]);
                        vis[xx][yy] = bfs_time;
                        Q1.emplace_back(xx, yy);
                    }
                }
            }
        }
        if (p1 == infty<int>) {
            swap(p1, p2), swap(Q1, Q2), swap(x1, x2), swap(y1, y2);
        }
        assert(p2 == infty<int>);
        // Q1 to new component
        int now = A[x1][y1];
        sz[now] -= len(Q1);
        for (auto& [x, y] : Q1) {
            A[x][y] = len(sz);
        }
        sz.emplace_back(len(Q1));
        n_comp++;
    }
};
} // namespace mitsuha
#endif // AJAY_GRID_DECREMENTAL_CONNECTIVITY
