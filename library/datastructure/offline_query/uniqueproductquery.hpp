#ifndef AJAY_UNIQUEPRODUCTQUERY
#define AJAY_UNIQUEPRODUCTQUERY

#include "library/datastructure/segment_tree/segtree.hpp"

namespace mitsuha{
// Calculate the total product of f(x) after UNIQUEing the elements (int) in [L, R).
// Query lookahead sort + seg tree
// add(L,R) all queries
// call as calc(f)
template <typename Mono>
struct UniqueProductQuery {
    using X = typename Mono::value_type;
    int N;
    vector<int> A;
    vector<pair<int, int>> query;

    UniqueProductQuery(vector<int>& A) : N(len(A)), A(A) {}

    void add(int L, int R) {
        assert(0 <= L && L <= R && R <= N);
        query.emplace_back(L, R);
    }

    template <typename F>
    vector<X> calc(F f) {
        long long Q = len(query);
        vector<X> ANS(Q);
        vector<vector<int>> IDS(N + 1);
        For(q, Q) IDS[query[q].second].emplace_back(q);
        SegTree<Mono> seg(N);

        unordered_map<int, int> pos;
        pos.reserve(N);

        for (auto&& q: IDS[0]) { ANS[q] = Mono::unit(); }
        For(i, N) {
            int x = A[i];
            if (pos.count(x)) { seg.set(pos[x], Mono::unit()); }
            pos[x] = i;
            seg.set(i, f(A[i]));
            for (auto&& q: IDS[i + 1]) {
                auto [L, R] = query[q];
                ANS[q] = seg.prod(L, R);
            }
        }
        return ANS;
    }

    vector<X> calc() {
        auto f = [&](long long k) -> X { return 1; };
        return calc(f);
    }
};
} // namespace mitsuha
#endif // AJAY_UNIQUEPRODUCTQUERY