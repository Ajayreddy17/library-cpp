#ifndef AJAY_BEATS_SUMMINMAX_CHMINCHMAX
#define AJAY_BEATS_SUMMINMAX_CHMINCHMAX

#include "library/datastructure/segment_tree/segtree_beats.hpp"

namespace mitsuha{
template <typename T>
struct Beats_SumMinMax_ChminChmax {
    struct SumMinMax {
        struct X {
            T sum, min, max, minc, maxc, min2, max2;
            bool fail;
        };
        using value_type = X;
        static X op(const X& x, const X& y) {
            if (x.min > x.max) return y;
            if (y.min > y.max) return x;
            X z;
            z.sum = x.sum + y.sum;

            z.min = min(x.min, y.min), z.max = max(x.max, y.max);
            z.minc = (x.min == z.min ? x.minc : 0) + (y.min == z.min ? y.minc : 0);
            z.maxc = (x.max == z.max ? x.maxc : 0) + (y.max == z.max ? y.maxc : 0);

            z.min2 = z.max;
            if (z.min < x.min && x.min < z.min2) z.min2 = x.min;
            if (z.min < x.min2 && x.min2 < z.min2) z.min2 = x.min2;
            if (z.min < y.min && y.min < z.min2) z.min2 = y.min;
            if (z.min < y.min2 && y.min2 < z.min2) z.min2 = y.min2;

            z.max2 = z.min;
            if (z.max > x.max && x.max > z.max2) z.max2 = x.max;
            if (z.max > x.max2 && x.max2 > z.max2) z.max2 = x.max2;
            if (z.max > y.max && y.max > z.max2) z.max2 = y.max;
            if (z.max > y.max2 && y.max2 > z.max2) z.max2 = y.max2;

            z.fail = 0;
            return z;
        }
        static constexpr X unit() {
            return {0, numeric_limits<T>::max() / 2, -(numeric_limits<T>::max() / 2), 0, 0, numeric_limits<T>::max() / 2, -(numeric_limits<T>::max() / 2), 0};
        }
        bool commute = true;
    };
    struct AddChminChmax {
        using X = tuple<T, T, T>;
        using value_type = X;
        static constexpr X op(const X& x, const X& y) {
            auto [a, b, c] = x;
            auto [d, e, f] = y;
            a += d, b += d, c += d;
            b = min(b, e), c = min(c, e), c = max(c, f);
            return {a, b, c};
        }
        static constexpr X unit() { return {0, numeric_limits<T>::max() / 2, -(numeric_limits<T>::max() / 2)}; }
        bool commute = false;
    };
    struct Beats {
        using Monoid_X = SumMinMax;
        using Monoid_A = AddChminChmax;
        using X = typename Monoid_X::value_type;
        using A = typename Monoid_A::value_type;
        static X act(X& x, const A& a, int cnt) {
            assert(!x.fail);
            if (x.min > x.max) return x;
            auto [add, mi, ma] = a;
            x.sum += cnt * add;
            x.min += add, x.max += add, x.min2 += add, x.max2 += add;

            if (mi == numeric_limits<T>::max() / 2 && ma == -(numeric_limits<T>::max() / 2)) return x;

            T before_min = x.min, before_max = x.max;
            x.min = min(x.min, mi), x.min = max(x.min, ma);
            x.max = min(x.max, mi), x.max = max(x.max, ma);

            if (x.min == x.max) {
                x.sum = x.max * cnt, x.max2 = x.min2 = x.max, x.maxc = x.minc = cnt;
            }
            else if (x.max2 <= x.min) {
                x.max2 = x.min, x.min2 = x.max, x.minc = cnt - x.maxc,
                x.sum = x.max * x.maxc + x.min * x.minc;
            }
            else if (x.min2 >= x.max) {
                x.max2 = x.min, x.min2 = x.max, x.maxc = cnt - x.minc,
                x.sum = x.max * x.maxc + x.min * x.minc;
            }
            else if (x.min < x.min2 && x.max > x.max2) {
                x.sum += (x.min - before_min) * x.minc + (x.max - before_max) * x.maxc;
            }
            else {
                x.fail = 1;
            }
            return x;
        }
    };

    using X = typename SumMinMax::X;
    SegTree_Beats<Beats> seg;
    Beats_SumMinMax_ChminChmax(vector<T>& A) {
        seg.build(len(A), [&](int i) -> X { return from_element(A[i]); });
    }
    template <typename F>
    Beats_SumMinMax_ChminChmax(int n, F f) {
        seg.build(n, [&](int i) -> X { return from_element(f(i)); });
    }
    void set(int i, T x) { seg.set(i, from_element(x)); }

    // (sum, max, min)
    tuple<T, T, T> prod(int l, int r) {
        auto e = seg.prod(l, r);
        return {e.sum, e.min, e.max};
    }
    static X from_element(T x) { return {x, x, x, 1, 1, x, x, 0}; }

    void chmin(int l, int r, T x) { seg.apply(l, r, {0, x, -(numeric_limits<T>::max() / 2)}); }
    void chmax(int l, int r, T x) { seg.apply(l, r, {0, numeric_limits<T>::max() / 2, x}); }
    void add(int l, int r, T x) { seg.apply(l, r, {x, numeric_limits<T>::max() / 2, -(numeric_limits<T>::max() / 2)}); }
};
} // namespace mitsuha
#endif // AJAY_BEATS_SUMMINMAX_CHMINCHMAX