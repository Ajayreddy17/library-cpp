#ifndef AJAY_COMPL_GRAPH_UNIONFIND
#define AJAY_COMPL_GRAPH_UNIONFIND

#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/datastructure/fastset.hpp"
#include "library/graph/base.hpp"

namespace mitsuha{
template <typename GT>
UnionFind complement_graph_unionfind(GT& G) {
    const int N = G.N;
    UnionFind uf(N);
    FastSet ss(N);
    For(v, N) ss.insert(v);

    vector<int> que;
    For(v, N) {
        if (!ss[v]) continue;
        que.emplace_back(v);
        while (len(que)) {
            int a = que.back(); que.pop_back();
            vector<int> tmp;
            for (auto&& e: G[a]) {
                if (ss[e.to]) tmp.emplace_back(e.to);
            }
            for (auto&& x: tmp) ss.erase(x);
            ss.enumerate(0, N, [&](int to) -> void {
                ss.erase(to);
                que.emplace_back(to);
                uf.merge(a, to);
            });
            for (auto&& x: tmp) ss.insert(x);
        }
    }
    return uf;
}
} // namespace mitsuha
#endif // AJAY_COMPL_GRAPH_UNIONFIND