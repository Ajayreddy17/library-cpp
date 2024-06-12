#ifndef AJAY_MAX_IND_SET
#define AJAY_MAX_IND_SET

#include "library/graph/base.hpp"

namespace mitsuha{
// The worst is N*1.381^N, but it may also be usable with N=100.
// MAX_V is max possible G.N
template <int MAX_V, typename GT>
vector<int> maximum_independent_set(GT& G) {
    using BS = bitset<MAX_V>;
    const int N = G.N;
    assert(N <= MAX_V);
    vector<BS> nbd(N);
    For(v, N) for (auto&& e: G[v]) nbd[v][e.to] = 1;

    int best = 0;
    BS res;

    auto dfs = [&](auto& dfs, BS now, BS rest) -> void {
        pair<int, int> p = {-1, -1}; // (v, d)
        while (1) {
            bool upd = 0;
            For(v, N) if (rest[v]) {
                int d = (nbd[v] & rest).count();
                if (chmax(p.second, d)) p.first = v;
                if (d <= 1) { rest[v] = 0, rest &= ~nbd[v], now[v] = 1, upd = 1; }
            }
            if (!upd) break;
            p = {-1, -1};
        }
        int a = now.count(), b = rest.count();
        if (chmax(best, a)) res = now;
        if (b == 0 || a + b <= best) return;
        int v = p.first;
        rest[v] = 0;
        if (p.second >= 3) dfs(dfs, now, rest);
        now[v] = 1;
        dfs(dfs, now, rest & ~(nbd[v]));
    };
    BS now, rest;
    For(v, N) rest[v] = 1;
    dfs(dfs, now, rest);
    vector<int> ANS;
    For(v, N) if (res[v]) ANS.emplace_back(v);
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_MAX_IND_SET