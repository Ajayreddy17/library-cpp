#ifndef AJAY_MAXIMUM_ANTICHAIN
#define AJAY_MAXIMUM_ANTICHAIN

#include "library/flow/bipartite_matching.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/maximum_antichain.hpp

// pass the comparability graph; a DAG alone is not enough
template <typename GT>
vector<int> maximum_antichain(GT& G) {
    static_assert(GT::is_directed);
    int n = G.N;

    Graph H(n + n);
    for (auto&& e : G.edges) {
        H.add(e.frm, e.to + n);
    }
    H.build();
    Bipartite_Matching BM(H);
    auto cover = BM.vertex_cover();
    auto match = BM.matching();
    assert(len(cover) == len(match));
    vector<bool> ok(n, 1);
    for (auto&& v : cover) {
        ok[v % n] = 0;
    }
    vector<int> antichain;
    For(v, n) if (ok[v]) antichain.emplace_back(v);
    for (auto&& e : G.edges) {
        assert(!ok[e.frm] || !ok[e.to]);
    }
    return antichain;
}
} // namespace mitsuha
#endif // AJAY_MAXIMUM_ANTICHAIN
