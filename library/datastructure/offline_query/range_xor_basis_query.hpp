#ifndef AJAY_RANGE_XOR_BASIS_QUERY
#define AJAY_RANGE_XOR_BASIS_QUERY

namespace mitsuha{
template <typename INT_TYPE, int LOG>
struct Range_Xor_Basis_Query {
    int n;
    int Q;
    vector<INT_TYPE>& A;
    vector<vector<pair<int, int>>> QUERY;

    Range_Xor_Basis_Query(vector<INT_TYPE>& A) : n(len(A)), Q(0), A(A) {
        QUERY.resize(n + 1);
    }

    void query(int L, int R) {
        assert(0 <= L && L <= R && R <= n);
        QUERY[R].emplace_back(Q++, L);
    }

    // function F(qid, basis)
    template <typename CALC>
    void calc(CALC F) {
        using P = pair<INT_TYPE, int>;
        array<P, LOG> d;
        For(k, LOG) d[k] = {INT_TYPE(1) << k, -1};
        vector<INT_TYPE> basis;
        For(i, n + 1) {
            for (auto&& [qid, L]: QUERY[i]) {
                basis.clear();
                Frr(k, LOG) if (d[k].second >= L) basis.emplace_back(d[k].first);
                F(qid, basis);
            }
            if (i == n) break;
            P p = {A[i], i};
            Frr(k, LOG) {
                if (!(p.first >> k & 1)) continue;
                if (p.second > d[k].second) swap(p, d[k]);
                p.first ^= d[k].first;
            }
        }
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_XOR_BASIS_QUERY