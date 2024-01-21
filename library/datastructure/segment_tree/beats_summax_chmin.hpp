#ifndef AJAY_BEATS_SUMMAX_CHMIN
#define AJAY_BEATS_SUMMAX_CHMIN

#include "library/datastructure/segment_tree/segtree_beats.hpp"

namespace mitsuha{
template <typename T>
struct Beats_SumMax_Chmin {
    struct SumMax {
        struct X {
            T sum, max, maxc, max2;
            bool fail;
        };
        using value_type = X;
        static X op(const X& x, const X& y) {
            if (x.max == -inf<T>) return y;
            if (y.max == -inf<T>) return x;
            X z;
            z.sum = x.sum + y.sum;
 
            z.max = max(x.max, y.max);
            z.maxc = (x.max == z.max ? x.maxc : 0) + (y.max == z.max ? y.maxc : 0);
 
            z.max2 = -inf<T>;
            if (z.max > x.max && x.max > z.max2) z.max2 = x.max;
            if (z.max > x.max2 && x.max2 > z.max2) z.max2 = x.max2;
            if (z.max > y.max && y.max > z.max2) z.max2 = y.max;
            if (z.max > y.max2 && y.max2 > z.max2) z.max2 = y.max2;
 
            z.fail = 0;
            return z;
        }
        static constexpr X unit() { return {0, -inf<T>, 0, -inf<T>, 0}; }
        bool commute = true;
    };
    struct AddChmin {
        using X = pair<T, T>;
        using value_type = X;
        static constexpr X op(const X& x, const X& y) {
            auto [a, b] = x;
            auto [d, e] = y;
            a += d, b += d, b = min(b, e);
            return {a, b};
        }
        static constexpr X unit() { return {0, inf<T>}; }
        bool commute = false;
    };
    struct Beats {
        using Monoid_X = SumMax;
        using Monoid_A = AddChmin;
        using X = typename Monoid_X::value_type;
        using A = typename Monoid_A::value_type;
        static X act(X& x, const A& a, int cnt) {
            assert(!x.fail);
            if (x.max == -inf<T>) return x;
            auto [add, mi] = a;
            x.sum += cnt * add, x.max += add, x.max2 += add;
 
            if (mi == inf<T>) return x;
 
            T before_max = x.max;
            x.max = min(x.max, mi);
            if (x.maxc == cnt) { x.max2 = x.max, x.sum = cnt * x.max; }
            elif (x.max2 < x.max) { x.sum += (x.max - before_max) * x.maxc; }
            else {
                x.fail = 1;
            }
            return x;
        }
    };
    using X = typename SumMax::X;
    SegTree_Beats<Beats> seg;
    Beats_SumMax_Chmin(vector<T>& A) {
        seg.build(len(A), [&](int i) -> X { return from_element(A[i]); });
    }
    template <typename F>
    Beats_SumMax_Chmin(int n, F f) {
        seg.build(n, [&](int i) -> X { return from_element(f(i)); });
    }
 
    void set(int i, T x) { seg.set(i, from_element(x)); }
 
    // (sum, max)
    pair<T, T> prod(int l, int r) {
        auto e = seg.prod(l, r);
        return {e.sum, e.max};
    }
    static X from_element(T x) { return {x, x, 1, x}; }
 
    void chmin(int l, int r, long long x) { seg.apply(l, r, {0, x}); }
    void add(int l, int r, long long x) { seg.apply(l, r, {x, inf<T>}); }
};
} // namespace mitsuha
#endif // AJAY_BEATS_SUMMAX_CHMIN