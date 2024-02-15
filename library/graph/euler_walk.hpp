#ifndef AJAY_EULER_WALK
#define AJAY_EULER_WALK

#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/graph/vs_to_es.hpp"

namespace mitsuha{
// (vs, es) or empty
template <typename GT>
pair<vector<int>, vector<int>> euler_walk(GT& G, int s = -1) {
    const int N = G.N, M = G.M;
    assert(G.is_prepared());
    assert(N > 0);

    if (s == -1) {
        vector<int> deg(N);
        for (auto&& e: G.edges) {
            if constexpr (GT::is_directed) {
                deg[e.frm]++, deg[e.to]--;
            } else {
                deg[e.frm]++, deg[e.to]++;
            }
        }
        if constexpr (GT::is_directed) {
            s = max_element(deg.begin(), deg.end()) - deg.begin();
            if (deg[s] == 0) s = (M == 0 ? 0 : G.edges[0].frm);
        } else {
            s = [&]() -> int {
                For(v, N) if (deg[v] & 1) return v;
                return (M == 0 ? 0 : G.edges[0].frm);
            }();
        }
    }

    if (M == 0) return {{s}, {}};
    vector<int> D(N), its(N), eu(M), vs, st = {s};
    For(v, N) its[v] = G.indptr[v];
    ++D[s];
    while (!st.empty()) {
        int x = st.back(), y, e, &it = its[x], end = G.indptr[x + 1];
        if (it == end) {
            vs.emplace_back(x);
            st.pop_back();
            continue;
        }
        auto& ee = G.csr_edges[it++];
        y = ee.to, e = ee.id;
        if (!eu[e]) {
            D[x]--, D[y]++;
            eu[e] = 1;
            st.emplace_back(y);
        }
    }
    for (auto&& x: D)
        if (x < 0) return {{}, {}};
    if (len(vs) != M + 1) return {{}, {}};
    reverse(vs.begin(), vs.end());
    auto es = vs_to_es(G, vs, false);
    return {vs, es};
}

template <typename GT>
bool has_euler_walk(GT& G, int s = -1) {
    int N = G.N, M = G.M;
    if (M == 0) return true;
    if constexpr (!GT::is_directed) {
        vector<int> odd(N);
        for (auto& e: G.edges) odd[e.frm] ^= 1, odd[e.to] ^= 1;
        int n_odd = 0;
        for (auto x: odd) n_odd += x;

        if (n_odd >= 4) return false;
        if (s != -1 && n_odd == 2 && !odd[s]) return false;
        UnionFind uf(N);
        for (auto& e: G.edges) uf.merge(e.frm, e.to);
        vector<int> cnt_edge(N);
        for (auto& e: G.edges) cnt_edge[uf[e.frm]]++;
        if (s != -1 && cnt_edge[uf[s]] == 0) return false;
        // Count components with edges
        int nc = 0;
        for (int v = 0; v < N; ++v) {
            if (uf[v] == v && cnt_edge[v] >= 1) ++nc;
        }
        return nc <= 1;
    } else {
        int N = G.N;
        vector<int> in(N), out(N);
        for (auto& e: G.edges) out[e.frm]++, in[e.to]++;

        int ng = 0;
        For(v, N) ng += abs(out[v] - in[v]);
        if (ng >= 4) return false;
        if (s != -1 && ng == 2 && out[s] != in[s] + 1) return false;

        UnionFind uf(N);
        for (auto& e: G.edges) uf.merge(e.frm, e.to);
        vector<int> cnt_edge(N);
        for (auto& e: G.edges) cnt_edge[uf[e.frm]]++;
        if (s != -1 && cnt_edge[uf[s]] == 0) return false;
        // Count components with edges
        int nc = 0;
        for (int v = 0; v < N; ++v) {
            if (uf[v] == v && cnt_edge[v] >= 1) ++nc;
        }
        return nc <= 1;
    }
}
} // namespace mitsuha
#endif // AJAY_EULER_WALK