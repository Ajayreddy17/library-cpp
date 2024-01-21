#ifndef AJAY_REVERSE_GRAPH
#define AJAY_REVERSE_GRAPH

#include "library/graph/base.hpp"

namespace mitsuha{
template <typename GT>
GT reverse_graph(GT& G) {
    static_assert(GT::is_directed);
    GT G1(G.N);
    for (auto&& e: G.edges) { G1.add(e.to, e.frm, e.cost, e.id); }
    G1.build();
    return G1;
}
} // namespace mitsuha
#endif // AJAY_REVERSE_GRAPH