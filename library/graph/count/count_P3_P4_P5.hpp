#ifndef AJAY_COUNT_P3_P4_P5
#define AJAY_COUNT_P3_P4_P5

#include "library/graph/count/count_C3_C4.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_P3_P4_P5.hpp

// for each v, count P3, P4, P5 starting at v (3, 4, 5 vertices)
// assumes a simple graph
template <typename GT>
tuple<vector<long long>, vector<long long>, vector<long long>> count_P3_P4_P5_pointwise(GT& G) {
    static_assert(!GT::is_directed);
    int N = G.N;
    auto deg = G.deg_array();
    auto [C3, C4] = count_C3_C4_pointwise(G);

    vector<long long> P3(N), P4(N), P5(N);

    For(v, N) {
        for (auto&& e: G[v]) {
            if (e.frm == e.to) continue;
            P3[v] += deg[e.to] - 1;
        }
    }

    For(v, N) {
        for (auto&& e: G[v]) {
            if (e.frm == e.to) continue;
            P4[v] += P3[e.to] - (deg[v] - 1);
        }
        P4[v] -= C3[v] * 2;
    }
    For(v, N) {
        for (auto&& e: G[v]) {
            if (e.frm == e.to) continue;
            P5[v] += P4[e.to];
        }
        P5[v] -= C4[v] * 2;
        P5[v] -= C3[v] * 2 * (deg[v] - 3);
        P5[v] -= P3[v] * (deg[v] - 1);
    }
    return {P3, P4, P5};
}
} // namespace mitsuha
#endif // AJAY_COUNT_P3_P4_P5
