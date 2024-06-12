#ifndef AJAY_VS_TO_ES
#define AJAY_VS_TO_ES

#include "library/datastructure/hashmap.hpp"

namespace mitsuha {
template <typename GT>
vector<int> vs_to_es(GT& G, vector<int>& vs, bool allow_use_twice = false) {
    assert(!vs.empty());

    HashMap<int> MP(G.M, -1);
    vector<int> nxt(G.M, -1);

    auto get = [&](long long a, long long b) -> unsigned long long{
        if (!GT::is_directed && a > b) swap(a, b);
        return a * G.N + b;
    };

    For(eid, G.M) {
        unsigned long long k = get(G.edges[eid].frm, G.edges[eid].to);
        int x = MP.get(k);
        nxt[eid] = x, MP[k] = eid;
    }
    int n = len(vs);
    vector<int> es(n - 1);
    For(i, n - 1) {
        unsigned long long k = get(vs[i], vs[i + 1]);
        int eid = MP.get(k);
        assert(eid != -1);
        es[i] = eid;
        if (!allow_use_twice) { MP[k] = nxt[eid]; }
    }
    return es;
}
} // namespace mitsuha
#endif // AJAY_VS_TO_ES