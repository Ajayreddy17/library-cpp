#ifndef AJAY_BIPARTITE_MATCHING_DENSE
#define AJAY_BIPARTITE_MATCHING_DENSE

#include "library/datastructure/bit_array.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/bipartite_matching_dense.hpp

// pass the bi-adjacency matrix as vector<bitset>
// N_1^2N_2/w.
//
// (5000, 5000) can run in about 300ms
// https://qoj.ac/problem/6308
//
// (10000?, 5000) 655ms
// https://codeforces.com/contest/1045/problem/A
//
// (10000, 20000): 3837ms
// https://codeforces.com/contest/786/problem/E
//
// (10000, 10000): 6500ms TLE; shuffling rows and cols gives 3000ms AC.
//
// small sizes like (26, 26): bitset is several times faster
// https://ac.nowcoder.com/acm/contest/133885/D
template <typename BS>
struct Bipartite_Matching_Dense {
    int N1, N2;
    vector<BS>& adj;
    vector<int> match_1, match_2;
    vector<int> que;
    vector<int> prev;
    BS vis;

    Bipartite_Matching_Dense(vector<BS>& adj, int N1, int N2)
            : N1(N1), N2(N2), adj(adj), match_1(N1, -1), match_2(N2, -1) {
        if constexpr (is_same_v<BS, Bit_Array>) vis.resize(N2);
        For(s, N1) bfs(s);
    }

    void bfs(int s) {
        if (match_1[s] != -1) return;
        que.resize(N1), prev.resize(N1);
        int l = 0, r = 0;
        prev[s] = -1;
        vis.set();

        que[r++] = s;
        while (l < r) {
            int u = que[l++];
            if constexpr (is_same_v<BS, Bit_Array>) {
                bool found = false;
                vis.enumerate_intersection(adj[u], [&](int v, bool& end) {
                    vis[v] = 0;
                    if (match_2[v] != -1) {
                        que[r++] = match_2[v];
                        prev[match_2[v]] = u;
                        return;
                    }
                    int a = u, b = v;
                    while (a != -1) {
                        int t = match_1[a];
                        match_1[a] = b, match_2[b] = a, a = prev[a], b = t;
                    }
                    found = end = true;
                });
                if (found) return;
            } else {
                BS cand = vis & adj[u];
                for (int v = cand._Find_first(); v < N2; v = cand._Find_next(v)) {
                    vis[v] = 0;
                    if (match_2[v] != -1) {
                        que[r++] = match_2[v];
                        prev[match_2[v]] = u;
                        continue;
                    }
                    int a = u, b = v;
                    while (a != -1) {
                        int t = match_1[a];
                        match_1[a] = b, match_2[b] = a, a = prev[a], b = t;
                    }
                    return;
                }
            }
        }
    }

    vector<pair<int, int>> matching() {
        vector<pair<int, int>> res;
        For(v, N1) if (match_1[v] != -1) res.emplace_back(v, match_1[v]);
        return res;
    }

    pair<vector<int>, vector<int>> vertex_cover() {
        vector<int> que(N1);
        int l = 0, r = 0;
        vis.set();
        vector<bool> done(N1);
        For(i, N1) {
            if (match_1[i] == -1) done[i] = 1, que[r++] = i;
        }
        while (l < r) {
            int a = que[l++];
            if constexpr (is_same_v<BS, Bit_Array>) {
                vis.enumerate_intersection(adj[a], [&](int b, bool&) {
                    vis[b] = 0;
                    int to = match_2[b];
                    assert(to != -1);
                    if (!done[to]) done[to] = 1, que[r++] = to;
                });
            } else {
                BS cand = adj[a] & vis;
                for (int b = cand._Find_first(); b < N2; b = cand._Find_next(b)) {
                    vis[b] = 0;
                    int to = match_2[b];
                    assert(to != -1);
                    if (!done[to]) done[to] = 1, que[r++] = to;
                }
            }
        }
        vector<int> left, right;
        For(i, N1) if (!done[i]) left.emplace_back(i);
        For(i, N2) if (!vis[i]) right.emplace_back(i);
        return {left, right};
    }
};
} // namespace mitsuha
#endif // AJAY_BIPARTITE_MATCHING_DENSE
