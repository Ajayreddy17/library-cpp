#ifndef AJAY_RANGE_MEX_QUERY
#define AJAY_RANGE_MEX_QUERY

#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_min.hpp"

namespace mitsuha{
template <int BEGIN = 0, typename T = long long>
struct Range_Mex_Query {
    vector<T>& A;
    vector<pair<int, int>> query;

    Range_Mex_Query(vector<T>& A) : A(A) {}
    
    void add(int l, int r) { query.emplace_back(l, r); }

    vector<T> calc() {
        int N = len(A);
        SegTree<Monoid_Min<int>> seg(N + 2, [](int){ return -1; });

        int Q = len(query);
        vector<T> ANS(Q);
        vector<vector<int>> IDS(N + 1);
        for(int q = 0; q < Q; ++q) {
            auto [L, R] = query[q];
            IDS[R].emplace_back(q);
        }

        for(int i = 0; i < N + 1; ++i) {
            for (auto&& q: IDS[i]) {
                int L = query[q].first;
                auto check = [&](int x) -> bool { return x >= L; };
                int mex = seg.max_right(BEGIN, check);
                ANS[q] = mex;
            }
            if (i < N && A[i] < N + 2) seg.set(A[i], i);
        }
        return ANS;
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_MEX_QUERY