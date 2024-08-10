#ifndef AJAY_WAVELET_MATRIX
#define AJAY_WAVELET_MATRIX

#include "library/monoid/monoid_add.hpp"
#include "library/datastructure/index_compression.hpp"
#include "library/datastructure/wavelet_matrix/bit_vector.hpp"

namespace mitsuha{
// Template to check existence of static method inverse
template <typename, typename = std::void_t<>>
struct has_inverse : std::false_type {};

template <typename T>
struct has_inverse<T, std::void_t<decltype(
T::inverse(std::declval<typename T::value_type>()))>>
        : std::true_type {};

struct Dummy_Data_Structure {
    using MX = Monoid_Add<bool>;
    void build(const vector<bool>& A) {}
};

template <typename Y, bool SMALL_Y, typename SEGTREE = Dummy_Data_Structure>
struct Wavelet_Matrix {
    using Mono = typename SEGTREE::MX;
    using T = typename Mono::value_type;
    static_assert(Mono::commute);

    int n, log, K;
    Index_Compression<Y, true, SMALL_Y> IDX;
    vector<Y> ItoY;
    vector<int> mid;
    vector<Bit_Vector> bv;
    vector<SEGTREE> seg;

    Wavelet_Matrix() {}
    Wavelet_Matrix(const vector<Y>& A) { build(A); }
    Wavelet_Matrix(const vector<Y>& A, vector<T>& SUM_Data) { build(A, SUM_Data); }
    template <typename F>
    Wavelet_Matrix(int n, F f) {
        build(n, f);
    }

    template <typename F>
    void build(int m, F f) {
        vector<Y> A(m);
        vector<T> S(m);
        for (int i = 0; i < m; ++i) tie(A[i], S[i]) = f(i);
        build(A, S);
    }

    void build(const vector<Y>& A) { build(A, vector<T>(len(A), Mono::unit())); }
    void build(const vector<Y>& A, vector<T> S) {
        n = len(A);
        vector<int> B = IDX.build(A);
        K = 0;
        for (auto& x: B) chmax(K, x + 1);
        ItoY.resize(K);
        For(i, n) ItoY[B[i]] = A[i];
        log = 0;
        while ((1 << log) < K) ++log;
        mid.resize(log), bv.assign(log, Bit_Vector(n));
        vector<int> B0(n), B1(n);
        vector<T> S0(n), S1(n);
        seg.resize(log + 1);
        seg[log].build(S);
        for (int d = log - 1; d >= 0; --d) {
            int p0 = 0, p1 = 0;
            for (int i = 0; i < n; ++i) {
                bool f = (B[i] >> d & 1);
                if (!f) { B0[p0] = B[i], S0[p0] = S[i], p0++; }
                if (f) { bv[d].set(i), B1[p1] = B[i], S1[p1] = S[i], p1++; }
            }
            swap(B, B0), swap(S, S0);
            move(B1.begin(), B1.begin() + p1, B.begin() + p0);
            move(S1.begin(), S1.begin() + p1, S.begin() + p0);
            mid[d] = p0, bv[d].build(), seg[d].build(S);
        }
    }

    // [L,R) x [-inf, y)
    int prefix_count(int L, int R, Y y) {
        int p = IDX(y);
        if (p == 0) return 0;
        if (p == K) return R - L;
        int cnt = 0;
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            if (p >> d & 1) cnt += r0 - l0, L = l1, R = r1;
            if (!(p >> d & 1)) L = l0, R = r0;
        }
        return cnt;
    }

    // [L,R) x [y1, y2)
    int count(int L, int R, Y y1, Y y2) {
        return prefix_count(L, R, y2) - prefix_count(L, R, y1);
    }

    // [L, R) x [-inf, y)
    pair<int, T> prefix_count_and_prod(int L, int R, Y y) {
        int p = IDX(y);
        if (p == 0) return {0, Mono::unit()};
        if (p == K) return {R - L, seg[log].prod(L, R)};
        int cnt = 0;
        T t = Mono::unit();
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            if (p >> d & 1) {
                cnt += r0 - l0, t = Mono::op(t, seg[d].prod(l0, r0)), L = l1, R = r1;
            }
            if (!(p >> d & 1)) L = l0, R = r0;
        }
        return {cnt, t};
    }

    // [L,R) x [y1, y2)
    pair<int, T> count_and_prod(int L, int R, Y y1, Y y2) {
        if constexpr (has_inverse<Mono>::value) {
            auto [c1, t1] = prefix_count_and_prod(L, R, y1);
            auto [c2, t2] = prefix_count_and_prod(L, R, y2);
            return {c2 - c1, Mono::op(Mono::inverse(t1), t2)};
        }
        int lo = IDX(y1), hi = IDX(y2), cnt = 0;
        T t = Mono::unit();
        auto dfs = [&](auto& dfs, int d, int L, int R, int a, int b) -> void {
            assert(b - a == (1 << d));
            if (hi <= a || b <= lo) return;
            if (lo <= a && b <= hi) {
                cnt += R - L, t = Mono::op(t, seg[d].prod(L, R));
                return;
            }
            --d;
            int c = (a + b) / 2;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            dfs(dfs, d, l0, r0, a, c), dfs(dfs, d, l1, r1, c, b);
        };
        dfs(dfs, log, L, R, 0, 1 << log);
        return {cnt, t};
    }

    // [L,R) x [-inf, y)
    T prefix_prod(int L, int R, Y y) { return prefix_count_and_prod(L, R, y).second; }
    // [L,R) x [y1, y2)
    T prod(int L, int R, Y y1, Y y2) { return count_and_prod(L, R, y1, y2).second; }
    T prod_all(int L, int R) { return seg[log].prod(L, R); }

    Y kth(int L, int R, int k) {
        assert(0 <= k && k < R - L);
        int p = 0;
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            if (k < r0 - l0) {
                L = l0, R = r0;
            } else {
                k -= r0 - l0, L = l1, R = r1, p |= 1 << d;
            }
        }
        return ItoY[p];
    }

    // numeric_limits<Y>::max() / 2 if no next
    Y next(int L, int R, Y y) {
        int k = IDX(y);
        int p = K;

        auto dfs = [&](auto& dfs, int d, int L, int R, int a, int b) -> void {
            if (p <= a || L == R || b <= k) return;
            if (d == 0) {
                chmin(p, a);
                return;
            }
            --d;
            int c = (a + b) / 2;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            dfs(dfs, d, l0, r0, a, c), dfs(dfs, d, l1, r1, c, b);
        };
        dfs(dfs, log, L, R, 0, 1 << log);
        return (p == K ? numeric_limits<Y>::max() / 2 : ItoY[p]);
    }

    // - numeric_limits<Y>::max() / 2 if no prev
    Y prev(int L, int R, Y y) {
        int k = IDX(y + 1);
        int p = -1;
        auto dfs = [&](auto& dfs, int d, int L, int R, int a, int b) -> void {
            if (b - 1 <= p || L == R || k <= a) return;
            if (d == 0) {
                chmax(p, a);
                return;
            }
            --d;
            int c = (a + b) / 2;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            dfs(dfs, d, l1, r1, c, b), dfs(dfs, d, l0, r0, a, c);
        };
        dfs(dfs, log, L, R, 0, 1 << log);
        return (p == -1 ? -(numeric_limits<Y>::max() / 2) : ItoY[p]);
    }

    Y median(bool UPPER, int L, int R) {
        assert(0 <= L && L < R && R <= n);
        int k = (UPPER ? (R - L) / 2 : (R - L - 1) / 2);
        return kth(L, R, k);
    }

    pair<Y, T> kth_value_and_prod(int L, int R, int k) {
        assert(0 <= k && k <= R - L);
        if (k == R - L) return {numeric_limits<Y>::max() / 2, seg[log].prod(L, R)};
        int p = 0;
        T t = Mono::unit();
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            if (k < r0 - l0) {
                L = l0, R = r0;
            } else {
                t = Mono::op(t, seg[d].prod(l0, r0)), k -= r0 - l0, L = l1, R = r1,
                p |= 1 << d;
            }
        }
        t = Mono::op(t, seg[0].prod(L, L + k));
        return {ItoY[p], t};
    }

    T prod_index_range(int L, int R, int k1, int k2) {
        static_assert(has_inverse<Mono>::value);
        T t1 = kth_value_and_prod(L, R, k1).second;
        T t2 = kth_value_and_prod(L, R, k2).second;
        return Mono::op(Mono::inverse(t1), t2);
    }

    // [L, R) x [-inf, y) The largest (cnt,prod) for which check(cnt, prod) is true
    template <typename F>
    pair<int, T> max_right(int L, int R, F check) {
        int cnt = 0;
        T t = Mono::unit();
        assert(check(0, Mono::unit()));
        if (check(R - L, seg[log].prod(L, R))) {
            return {R - L, seg[log].prod(L, R)};
        }
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            int cnt1 = cnt + r0 - l0;
            T t1 = Mono::op(t, seg[d].prod(l0, r0));
            if (check(cnt1, t1)) {
                cnt = cnt1, t = t1, L = l1, R = r1;
            } else {
                L = l0, R = r0;
            }
        }
        return {cnt, t};
    }

    void set(int i, T t) {
        assert(0 <= i && i < n);
        int L = i, R = i + 1;
        seg[log].set(L, t);
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            if (l0 < r0) L = l0, R = r0;
            if (l0 == r0) L = l1, R = r1;
            seg[d].set(L, t);
        }
    }
    void multiply(int i, T t) {
        assert(0 <= i && i < n);
        int L = i, R = i + 1;
        seg[log].multiply(L, t);
        for (int d = log - 1; d >= 0; --d) {
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int l1 = L + mid[d] - l0, r1 = R + mid[d] - r0;
            if (l0 < r0) L = l0, R = r0;
            if (l0 == r0) L = l1, R = r1;
            seg[d].multiply(L, t);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_WAVELET_MATRIX