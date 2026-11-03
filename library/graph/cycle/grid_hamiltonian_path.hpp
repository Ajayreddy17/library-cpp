#ifndef AJAY_GRID_HAMILTONIAN_PATH
#define AJAY_GRID_HAMILTONIAN_PATH

namespace mitsuha{
// Create a Hamilton path by fixing the starting point. If it's an even number, make it a cycle.
// High-dimensional version: https://codeforces.com/contest/863/submission/194294053
vector<pair<int, int>> grid_hamiltonian_path(int H, int W, int sx = 0, int sy = 0) {
    using P = pair<int, int>;
    if (H == 1) {
        vector<P> path;
        if (sy == 0) {
            For(y, W) path.emplace_back(0, y);
            return path;
        }
        if (sy == W - 1) {
            Frr(y, W) path.emplace_back(0, y);
            return path;
        }
        return {};
    }
    if (W == 1) {
        vector<P> path;
        if (sx == 0) {
            For(x, H) path.emplace_back(x, 0);
            return path;
        }
        if (sx == H - 1) {
            Frr(x, H) path.emplace_back(x, 0);
            return path;
        }
        return {};
    }
    if (H % 2 == 0 && W % 2 == 1) {
        vector<P> path = grid_hamiltonian_path(W, H, sy, sx);
        for (auto&& x : path)
            swap(x.first, x.second);
        return path;
    }
    if (W % 2 == 0) {
        vector<P> path;
        path.reserve(H * W);
        For(j, W) path.emplace_back(0, j);
        Frr(j, W) {
            if (j % 2 == 1)
                For(i, 1, H) path.emplace_back(i, j);
            if (j % 2 == 0)
                Frr(i, 1, H) path.emplace_back(i, j);
        }
        int idx = -1;
        For(i, len(path)) if (path[i].first == sx && path[i].second == sy) idx = i;
        rotate(path.begin(), path.begin() + idx, path.end());
        return path;
    }

    assert(H * W % 2 == 1);
    if ((sx + sy) % 2 == 1)
        return {};
    if (sx % 2 == 1) {
        vector<P> path;
        path.reserve(H * W);
        Frr(i, sx + 1) {
            if (i % 2 == 1)
                Frr(j, sy + 1) path.emplace_back(i, j);
            if (i % 2 == 0)
                For(j, sy + 1) path.emplace_back(i, j);
        }
        For(j, sy + 1, W) {
            if (j % 2 == 0)
                For(i, sx + 1) path.emplace_back(i, j);
            if (j % 2 == 1)
                Frr(i, sx + 1) path.emplace_back(i, j);
        }
        Frr(j, W) {
            if (j % 2 == 0)
                For(i, sx + 1, H) path.emplace_back(i, j);
            if (j % 2 == 1)
                Frr(i, sx + 1, H) path.emplace_back(i, j);
        }
        return path;
    }
    if (sx == H - 1) {
        vector<P> path = grid_hamiltonian_path(H, W, 0, sy);
        for (auto&& x : path)
            x.first = H - 1 - x.first;
        return path;
    }
    if (sy == W - 1) {
        vector<P> path = grid_hamiltonian_path(H, W, sx, 0);
        for (auto&& x : path)
            x.second = W - 1 - x.second;
        return path;
    }
    if (sx != 0 && sy == 0) {
        vector<P> path = grid_hamiltonian_path(W, H, sy, sx);
        for (auto&& x : path)
            swap(x.first, x.second);
        return path;
    }
    vector<P> path;
    path.reserve(H * W);
    if (sx == 0) {
        Frr(j, sy + 1) {
            if (j % 2 == 0)
                For(i, H - 1) path.emplace_back(i, j);
            if (j % 2 == 1)
                Frr(i, H - 1) path.emplace_back(i, j);
        }
        For(j, W) path.emplace_back(H - 1, j);
        Frr(j, sy + 1, W) {
            if (j % 2 == 0)
                Frr(i, H - 1) path.emplace_back(i, j);
            if (j % 2 == 1)
                For(i, H - 1) path.emplace_back(i, j);
        }
        return path;
    }
    assert(0 < sx && sx < H - 1 && 0 < sy && sy < W - 1);
    Frr(j, sy + 1) path.emplace_back(sx, j);
    For(j, sy + 1) {
        if (j % 2 == 0)
            Frr(i, sx) path.emplace_back(i, j);
        if (j % 2 == 1)
            For(i, sx) path.emplace_back(i, j);
    }
    For(i, sx + 1) {
        if (i % 2 == 0)
            For(j, sy + 1, W) path.emplace_back(i, j);
        if (i % 2 == 1)
            Frr(j, sy + 1, W) path.emplace_back(i, j);
    }
    Frr(j, W) {
        if (j % 2 == 0)
            For(i, sx + 1, H) path.emplace_back(i, j);
        if (j % 2 == 1)
            Frr(i, sx + 1, H) path.emplace_back(i, j);
    }
    return path;
}
} // namespace mitsuha
#endif // AJAY_GRID_HAMILTONIAN_PATH