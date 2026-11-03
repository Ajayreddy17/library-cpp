#ifndef AJAY_ST_NUMBERING
#define AJAY_ST_NUMBERING

#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/graph/connectivity/block_cut.hpp"
#include "library/graph/shortestpath/bfs01.hpp"
#include "library/graph/restore_path.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/connectivity/st_numbering.hpp


// https://en.wikipedia.org/wiki/Bipolar_orientation
// find a permutation p with p[s]=0, p[t]=n-1
// orienting edges by p[u]<p[v], every v lies on some s-v-t path
// exists iff some s-t path passes through every non-loop component of the BCT
// returns empty if impossible
template <typename GT>
vector<int> st_numbering(GT &G, int s, int t) {
    static_assert(!GT::is_directed);
    assert(G.is_prepared());
    int N = G.N;
    if (N == 1) return {0};
    if (s == t) return {};
    vector<int> par(N, -1), pre(N, -1), low(N, -1);
    vector<int> V;

    auto dfs = [&](auto &dfs, int v) -> void {
        pre[v] = len(V), V.emplace_back(v);
        low[v] = v;
        for (auto &e : G[v]) {
            int w = e.to;
            if (v == w) continue;
            if (pre[w] == -1) {
                dfs(dfs, w);
                par[w] = v;
                if (pre[low[w]] < pre[low[v]]) {
                    low[v] = low[w];
                }
            }
            elif (pre[w] < pre[low[v]]) { low[v] = w; }
        }
    };

    pre[s] = 0, V.emplace_back(s);
    dfs(dfs, t);
    if (len(V) != N) return {};
    vector<int> nxt(N, -1), prev(N);
    nxt[s] = t, prev[t] = s;

    vector<int> sgn(N);
    sgn[s] = -1;
    For(i, 2, len(V)) {
        int v = V[i];
        int p = par[v];
        if (sgn[low[v]] == -1) {
            int q = prev[p];
            if (q == -1) return {};
            nxt[q] = v, nxt[v] = p;
            prev[v] = q, prev[p] = v;
            sgn[p] = 1;
        } else {
            int q = nxt[p];
            if (q == -1) return {};
            nxt[p] = v, nxt[v] = q;
            prev[v] = p, prev[q] = v;
            sgn[p] = -1;
        }
    }
    vector<int> A = {s};
    while (A.back() != t) {
        A.emplace_back(nxt[A.back()]);
    }
    // check the result
    if (len(A) < N) return {};
    assert(A[0] == s && A.back() == t);
    vector<int> rk(N, -1);
    For(i, N) rk[A[i]] = i;
    assert(*min_element(rk.begin(), rk.end()) != -1);
    For(i, N) {
        bool l = 0, r = 0;
        int v = A[i];
        for (auto &e : G[v]) {
            if (rk[e.to] < rk[v]) l = 1;
            if (rk[v] < rk[e.to]) r = 1;
        }
        if (i > 0 && !l) return {};
        if (i < N - 1 && !r) return {};
    }
    vector<int> res(N);
    For(i, N) res[A[i]] = i;
    return res;
}
} // namespace mitsuha
#endif // AJAY_ST_NUMBERING
