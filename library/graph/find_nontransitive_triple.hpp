#ifndef AJAY_FIND_NONTRANSITIVE_TRIPLE
#define AJAY_FIND_NONTRANSITIVE_TRIPLE

#include "library/graph/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/find_nontransitive_triple.hpp

// find a triple (a,b,c) with edges ab, bc but no edge ac
// {-1,-1,-1} if none
// impossible: complete graph, or all edges independent
// example: https://codeforces.com/contest/967/problem/F
template <typename Graph>
tuple<int, int, int> find_nontransitive_triple(Graph& G) {
    int N = G.N;
    assert(G.is_prepared());
    assert(!G.is_directed());
    vector<int> done(N);
    vector<int> que;
    For(root, N) {
        if (done[root]) continue;
        que = {root};
        int p = 0;
        while (p < len(que)) {
            int v = que[p++];
            done[v] = 2;
            long long s = 0;
            for (auto&& e: G[v]) {
                int to = e.to;
                if (done[to] == 0) {
                    done[to] = 1;
                    que.emplace_back(to);
                }
                elif (done[to] == 2) { ++s; }
            }
            if (s == p - 1) continue;
            assert(p >= 3);
            int c = v;
            int a = -1;
            int b = -1;
            for (auto&& e: G[v]) done[e.to] = 0;
            For(i, p - 1) {
                int x = que[i];
                if (done[x] == 2) a = x;
                if (done[x] == 0) b = x;
            }
            assert(a != -1);
            assert(b != -1);
            return {a, b, c};
        }
    }
    return {-1, -1, -1};
}
} // namespace mitsuha
#endif // AJAY_FIND_NONTRANSITIVE_TRIPLE
