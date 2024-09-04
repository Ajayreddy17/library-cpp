#ifndef AJAY_CENTROID_DECOMPOSITION
#define AJAY_CENTROID_DECOMPOSITION

#include "library/graph/shortestpath/bfs01.hpp"

namespace mitsuha{
// Centroid decomposition of vertex-based centroid decomposition
// f(par, V, indptr)
template <typename F>
void centroid_decomposition_0_dfs(vector<int>& par, vector<int>& vs, F f) {
    const int N = len(par);
    assert(N >= 1);
    int c = -1;
    vector<int> sz(N, 1);
    Frr(i, N) {
        if (sz[i] >= cld<int>(N, 2)) {
            c = i;
            break;
        }
        sz[par[i]] += sz[i];
    }
    vector<int> color(N);
    vector<int> V = {c};
    int nc = 1;
    For(v, 1, N) {
        if (par[v] == c) { V.emplace_back(v), color[v] = nc++; }
    }
    if (c > 0) {
        for (int a = par[c]; a != -1; a = par[a]) { color[a] = nc, V.emplace_back(a); }
        ++nc;
    }
    For(i, N) {
        if (i != c && color[i] == 0) color[i] = color[par[i]], V.emplace_back(i);
    }
    vector<int> indptr(nc + 1);
    For(i, N) indptr[1 + color[i]]++;
    For(i, nc) indptr[i + 1] += indptr[i];
    vector<int> counter = indptr;
    vector<int> ord(N);
    for (auto& v: V) { ord[counter[color[v]]++] = v; }
    vector<int> new_idx(N);
    For(i, N) new_idx[ord[i]] = i;
    vector<int> name(N);
    For(i, N) name[new_idx[i]] = vs[i];
    {
        vector<int> tmp(N, -1);
        For(i, 1, N) {
            int a = new_idx[i], b = new_idx[par[i]];
            if (a > b) swap(a, b);
            tmp[b] = a;
        }
        swap(par, tmp);
    }
    f(par, name, indptr);
    For(k, 1, nc) {
        int L = indptr[k], R = indptr[k + 1];
        vector<int> par1(R - L, -1);
        vector<int> name1(R - L, -1);
        name1[0] = name[0];
        For(i, L, R) name1[i - L] = name[i];
        For(i, L, R) { par1[i - L] = max(par[i] - L, -1); }
        centroid_decomposition_0_dfs(par1, name1, f);
    }
}

/*
https://maspypy.com/%e9%87%8d%e5%bf%83%e5%88%86%e8%a7%a3%e3%83%bb1-3%e9%87%8d%e5%bf%83%e5%88%86%e8%a7%a3%e3%81%ae%e3%81%8a%e7%b5%b5%e6%8f%8f%e3%81%8d
centroid_decomposition_1：Entire path of length 2 or more
f(par, V, n1, n2)
[1,1+n1]: color 1
[1+n1,1+n1+n2]: color 2
*/
template <typename F>
void centroid_decomposition_1_dfs(vector<int>& par, vector<int> vs, F f) {
    const int N = len(par);
    assert(N > 1);
    if (N == 2) { return; }
    int c = -1;
    vector<int> sz(N, 1);
    Frr(i, N) {
        if (sz[i] >= cld<int>(N, 2)) {
            c = i;
            break;
        }
        sz[par[i]] += sz[i];
    }
    vector<int> color(N, -1);
    int take = 0;
    vector<int> ord(N, -1);
    ord[c] = 0;
    int p = 1;
    For(v, 1, N) {
        if (par[v] == c && take + sz[v] <= fld<int>(N - 1, 2)) {
            color[v] = 0, ord[v] = p++, take += sz[v];
        }
    }
    For(i, 1, N) {
        if (color[par[i]] == 0) color[i] = 0, ord[i] = p++;
    }
    int n0 = p - 1;
    for (int a = par[c]; a != -1; a = par[a]) { color[a] = 1, ord[a] = p++; }
    For(i, N) {
        if (i != c && color[i] == -1) color[i] = 1, ord[i] = p++;
    }
    assert(p == N);
    int n1 = N - 1 - n0;
    vector<int> par0(n0 + 1, -1), par1(n1 + 1, -1), par2(N, -1);
    vector<int> V0(n0 + 1), V1(n1 + 1), V2(N);
    For(v, N) {
        int i = ord[v];
        V2[i] = vs[v];
        if (color[v] != 1) { V0[i] = vs[v]; }
        if (color[v] != 0) { V1[max(i - n0, 0)] = vs[v]; }
    }
    For(v, 1, N) {
        int a = ord[v], b = ord[par[v]];
        if (a > b) swap(a, b);
        par2[b] = a;
        if (color[v] != 1 && color[par[v]] != 1) par0[b] = a;
        if (color[v] != 0 && color[par[v]] != 0)
            par1[max(b - n0, 0)] = max(a - n0, 0);
    }
    f(par2, V2, n0, n1);
    centroid_decomposition_1_dfs(par0, V0, f);
    centroid_decomposition_1_dfs(par1, V1, f);
}

/*
https://maspypy.com/%e9%87%8d%e5%bf%83%e5%88%86%e8%a7%a3%e3%83%bb1-3%e9%87%8d%e5%bf%83%e5%88%86%e8%a7%a3%e3%81%ae%e3%81%8a%e7%b5%b5%e6%8f%8f%e3%81%8d
f(par, V, color)
color in [-1,0,1], -1 is virtual.
*/
template <typename F>
void centroid_decomposition_2_dfs(vector<int>& par, vector<int>& vs, vector<int>& real, F f) {
    const int N = len(par);
    assert(N > 1);
    if (N == 2) {
        if (real[0] && real[1]) {
            vector<int> color = {0, 1};
            f(par, vs, color);
        }
        return;
    }
    int c = -1;
    vector<int> sz(N, 1);
    Frr(i, N) {
        if (sz[i] >= cld<int>(N, 2)) {
            c = i;
            break;
        }
        sz[par[i]] += sz[i];
    }
    vector<int> color(N, -1);
    int take = 0;
    vector<int> ord(N, -1);
    ord[c] = 0;
    int p = 1;
    For(v, 1, N) {
        if (par[v] == c && take + sz[v] <= fld<int>(N - 1, 2)) {
            color[v] = 0, ord[v] = p++, take += sz[v];
        }
    }
    For(i, 1, N) {
        if (color[par[i]] == 0) color[i] = 0, ord[i] = p++;
    }
    int n0 = p - 1;
    for (int a = par[c]; a != -1; a = par[a]) { color[a] = 1, ord[a] = p++; }
    For(i, N) {
        if (i != c && color[i] == -1) color[i] = 1, ord[i] = p++;
    }
    assert(p == N);
    int n1 = N - 1 - n0;
    vector<int> par0(n0 + 1, -1), par1(n1 + 1, -1), par2(N, -1);
    vector<int> V0(n0 + 1), V1(n1 + 1), V2(N);
    vector<int> rea0(n0 + 1), rea1(n1 + 1), rea2(N);
    For(v, N) {
        int i = ord[v];
        V2[i] = vs[v], rea2[i] = real[v];
        if (color[v] != 1) { V0[i] = vs[v], rea0[i] = real[v]; }
        if (color[v] != 0) {
            V1[max(i - n0, 0)] = vs[v], rea1[max(i - n0, 0)] = real[v];
        }
    }
    For(v, 1, N) {
        int a = ord[v], b = ord[par[v]];
        if (a > b) swap(a, b);
        par2[b] = a;
        if (color[v] != 1 && color[par[v]] != 1) par0[b] = a;
        if (color[v] != 0 && color[par[v]] != 0)
            par1[max(b - n0, 0)] = max(a - n0, 0);
    }
    // if (real[c]) {
    //   color.assign(N, -1);
    //   color[0] = 0;
    //   FOR(i, 1, N) color[i] = rea2[i] ? 1 : -1;
    //   f(par2, V2, color);
    //   rea0[0] = rea1[0] = rea2[0] = 0;
    // }
    color.assign(N, -1);
    For(i, 1, N) if (rea2[i]) color[i] = (i <= n0 ? 0 : 1);
    if (real[c]) color[0] = 2, rea0[0] = rea1[0] = rea2[0] = 0;
    f(par2, V2, color);
    centroid_decomposition_2_dfs(par0, V0, rea0, f);
    centroid_decomposition_2_dfs(par1, V1, rea1, f);
}

// 0: f(par, V, indptr)
// 1: f(par, V, n1, n2)
// 2: f(par, V, color)
template <int MODE, typename GT, typename F>
void centroid_decomposition(GT& G, F f) {
    static_assert(!GT::is_directed);
    const int N = G.N;
    if (N == 1) return;
    vector<int> V(N), par(N, -1);
    int l = 0, r = 0;
    V[r++] = 0;
    while (l < r) {
        int v = V[l++];
        for (auto& e: G[v]) {
            if (e.to != par[v]) V[r++] = e.to, par[e.to] = v;
        }
    }
    assert(r == N);
    vector<int> new_idx(N);
    For(i, N) new_idx[V[i]] = i;
    vector<int> tmp(N, -1);
    For(i, 1, N) {
        int j = par[i];
        tmp[new_idx[i]] = new_idx[j];
    }
    swap(par, tmp);
    static_assert(MODE == 0 || MODE == 1 || MODE == 2);
    if constexpr (MODE == 0) { centroid_decomposition_0_dfs(par, V, f); }
    elif constexpr(MODE == 1) { centroid_decomposition_1_dfs(par, V, f); }
    else {
        vector<int> real(N, 1);
        centroid_decomposition_2_dfs(par, V, real, f);
    }
}
} // namespace mitsuha
#endif // AJAY_CENTROID_DECOMPOSITION