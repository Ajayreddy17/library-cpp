#ifndef AJAY_COEF_QUERY_2D
#define AJAY_COEF_QUERY_2D

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

namespace mitsuha{
// A, B: constant
// Give Sparse Laurent Polynomial f(x,y)
// [x^py^q] Find many f(x,y)/(1-x)^A(1-y)^B
// O(AB N logN) time
template <int A, int B, typename T, typename XY>
struct Coefficient_Query_2D {
    struct Mono {
        using value_type = array<T, A * B>;
        using X = value_type;
        static X op(X x, X y) {
            for(int i = 0; i < A * B; ++i) x[i] += y[i];
            return x;
        }
        static constexpr X unit() { return X{}; }
        static constexpr bool commute = 1;
    };

    vector<tuple<XY, XY, T>> F;
    vector<pair<XY, XY>> QUERY;

    Coefficient_Query_2D() {}
    void add_query(XY x, XY y, T c) { F.emplace_back(x, y, c); }
    void sum_query(XY p, XY q) { QUERY.emplace_back(p, q); }

    vector<T> calc(bool div_fact = true) {
        sort(F.begin(), F.end(), [&](auto& a, auto& b) -> bool { return get<0>(a) < get<0>(b); });
        vector<XY> keyX;
        keyX.reserve(len(F));
        for (auto&& [a, b, c]: F) {
            if (keyX.empty() || keyX.back() != a) keyX.emplace_back(a);
            a = len(keyX) - 1;
        }
        keyX.shrink_to_fit();
        const int Q = len(QUERY);
        vector<int> I(Q);
        iota(I.begin(), I.end(), 0);
        sort(I.begin(), I.end(), [&](auto& a, auto& b) -> bool { return QUERY[a].second < QUERY[b].second; });
        sort(F.begin(), F.end(), [&](auto& a, auto& b) -> bool { return get<1>(a) < get<1>(b); });
        FenwickTree<Mono> bit(len(keyX));
        vector<T> res(Q);
        int ptr = 0;
        for (auto&& qid: I) {
            auto [p, q] = QUERY[qid];
            while (ptr < len(F) && get<1>(F[ptr]) <= q) {
                auto& [ia, b, w] = F[ptr++];
                XY a = keyX[ia];
                vector<T> f(A), g(B);
                f[0] = w, g[0] = 1;
                for(int i = 0; i < A - 1; ++i) { for(int j = i; j >= 0; --j) f[j + 1] += f[j] * T(-a + 1 + i); }
                for(int i = 0; i < B - 1; ++i) { for(int j = i; j >= 0; --j) g[j + 1] += g[j] * T(-b + 1 + i); }
                reverse(f.begin(), f.end()), reverse(g.begin(), g.end());
                array<T, A * B> G{};
                for(int i = 0; i < A; ++i) for(int j = 0; j < B; ++j) G[B * i + j] = f[i] * g[j];
                bit.add(ia, G);
            }
            auto SM = bit.sum(upper_bound(keyX.begin(), keyX.end(), p) - keyX.begin());
            T sm = 0, pow_p = 1;
            for(int i = 0; i < A; ++i) {
                T prod = pow_p;
                for(int j = 0; j < B; ++j) { sm += prod * SM[B * i + j], prod *= T(q); }
                pow_p *= T(p);
            }
            res[qid] = sm;
        }
        if (div_fact && (A >= 3 || B >= 3)) {
            T cf = T(1);
            for(int a = 1; a < A; ++a) cf *= T(a);
            for(int b = 1; b < B; ++b) cf *= T(b);
            for (auto&& x: res) x /= cf;
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_COEF_QUERY_2D