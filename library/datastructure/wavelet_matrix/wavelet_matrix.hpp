#ifndef AJAY_WAVELET_MATRIX
#define AJAY_WAVELET_MATRIX

#include "library/datastructure/wavelet_matrix/bit_vector.hpp"
#include "library/datastructure/dummy_data_structure.hpp"

namespace mitsuha{
// Values A[i] in [0, 2^log), each with a weight dat[i] stored in SEGTREE (one per level).
// SEGTREE needs MX (commutative monoid), build(vector), prod(l, r) const;
// set / multiply / add only if those updates are used.
// Queries over [L, R) x [y1, y2): O(log) levels, O(log) SEGTREE products.
template <typename Y, typename SEGTREE>
struct Uncompressed_Wavelet_Matrix {
    using Mono = typename SEGTREE::MX;
    using T = typename Mono::value_type;
    static_assert(Mono::commute);
    static_assert(is_same_v<Y, int> || is_same_v<Y, long long>);
    static constexpr Y INF = numeric_limits<Y>::max() / 2;
    int n = 0, log = 0;
    vector<int> mid;
    vector<Bit_Vector> bv;
    vector<SEGTREE> seg;
    Y limit;

    Uncompressed_Wavelet_Matrix() = default;

    // f(i) = {A[i], dat[i]}
    template <typename F>
    Uncompressed_Wavelet_Matrix(int n, F f, int log = -1) {
        build(n, f, log);
    }
    Uncompressed_Wavelet_Matrix(const vector<Y>& A, int log = -1) { build(A, log); }

    void build(const vector<Y>& A, int log = -1) {
        static_assert(is_same_v<SEGTREE, Dummy_Data_Structure>);
        build(len(A), [&](int i) -> pair<Y, T> { return {A[i], Mono::unit()}; }, log);
    }

    template <typename F>
    void build(int n, F f, int log = -1) {
        this->n = n;
        vector<Y> A(n);
        vector<T> S(n);
        For(i, n) tie(A[i], S[i]) = f(i);
        for (auto& x: A) assert(x >= 0);
        if (log == -1) {
            log = 0;
            for (auto& x: A) while ((Y(1) << log) <= x) ++log;
        } else {
            for (auto& x: A) assert(x < (Y(1) << log));
        }
        this->log = log;
        limit = Y(1) << log;
        if constexpr (is_same_v<Y, int>) assert(0 <= log && log <= 30);
        if constexpr (is_same_v<Y, long long>) assert(0 <= log && log <= 62);
        mid.resize(log), bv.assign(log, Bit_Vector(n));
        vector<Y> A0(n), A1(n);
        vector<T> S0(n), S1(n);
        seg.resize(log + 1);
        seg[log].build(S);
        for (int d = log - 1; d >= 0; --d) {
            int p0 = 0, p1 = 0;
            for (int i = 0; i < n; ++i) {
                if (A[i] >> d & 1) {
                    bv[d].set(i), A1[p1] = A[i], S1[p1] = S[i], p1++;
                } else {
                    A0[p0] = A[i], S0[p0] = S[i], p0++;
                }
            }
            swap(A, A0), swap(S, S0);
            move(A1.begin(), A1.begin() + p1, A.begin() + p0);
            move(S1.begin(), S1.begin() + p1, S.begin() + p0);
            mid[d] = p0, bv[d].build(), seg[d].build(S);
        }
    }

    // [L, R) at level d -> its 0-child [L0, R0) and 1-child [L1, R1) at level d - 1
    tuple<int, int, int, int> get_subtree(int d, int L, int R) const {
        assert(1 <= d && d <= log);
        int a = bv[d - 1].count_prefix(L), b = bv[d - 1].count_prefix(R);
        return {L - a, R - b, mid[d - 1] + a, mid[d - 1] + b};
    }

    // f(d, j): position j of A[i] at every level d = log, ..., 0
    template <typename F>
    void work_point(F f, int i) {
        assert(0 <= i && i < n);
        f(log, i);
        Frr(d, log) {
            int a = bv[d].count_prefix(i);
            i = (bv[d][i] ? mid[d] + a : i - a);
            f(d, i);
        }
    }

    // f(d, l, r): disjoint nodes covering [L, R) x [0, y)
    template <typename F>
    void work_prefix(F f, int L, int R, Y y) const {
        chmin(y, limit);
        if (y <= 0) return;
        if (y == limit) {
            f(log, L, R);
            return;
        }
        Frr(d, log) {
            auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
            if (y >> d & 1) {
                f(d, L0, R0);
                L = L1, R = R1;
            } else {
                L = L0, R = R0;
            }
        }
    }

    // f(d, l, r): disjoint nodes covering [L, R) x [y1, y2)
    template <typename F>
    void work_range(F f, int L, int R, Y y1, Y y2) const {
        chmax(y1, Y(0)), chmin(y2, limit);
        if (y1 >= y2) return;
        if (y1 == 0) return work_prefix(f, L, R, y2);
        auto dfs = [&](auto& dfs, int d, int L, int R, Y y1, Y y2) -> void {
            if (y1 == y2) return;
            if (y1 == 0 && y2 == Y(1) << d) {
                f(d, L, R);
                return;
            }
            assert(d > 0);
            auto [L0, R0, L1, R1] = get_subtree(d, L, R);
            Y m = Y(1) << (d - 1);
            if (y2 <= m) {
                dfs(dfs, d - 1, L0, R0, y1, y2);
            } else if (y1 >= m) {
                dfs(dfs, d - 1, L1, R1, y1 - m, y2 - m);
            } else {
                dfs(dfs, d - 1, L0, R0, y1, m);
                dfs(dfs, d - 1, L1, R1, 0, y2 - m);
            }
        };
        dfs(dfs, log, L, R, y1, y2);
    }

    // [L, R) x [0, y)
    int prefix_count(int L, int R, Y y) const {
        int cnt = 0;
        work_prefix([&](int, int a, int b) { cnt += b - a; }, L, R, y);
        return cnt;
    }

    // [L, R) x [y1, y2)
    int count(int L, int R, Y y1, Y y2) const {
        if (y1 >= y2) return 0;
        return prefix_count(L, R, y2) - prefix_count(L, R, y1);
    }

    // [L, R) x [0, y)
    T prefix_prod(int L, int R, Y y) const {
        T ans = Mono::unit();
        work_prefix([&](int d, int a, int b) { ans = Mono::op(ans, seg[d].prod(a, b)); }, L, R, y);
        return ans;
    }

    // [L, R) x [y1, y2)
    T prod(int L, int R, Y y1, Y y2) const {
        T ans = Mono::unit();
        work_range([&](int d, int a, int b) { ans = Mono::op(ans, seg[d].prod(a, b)); }, L, R, y1, y2);
        return ans;
    }

    T prod_all(int L, int R) const { return seg[log].prod(L, R); }

    // [L, R) x [0, y)
    pair<int, T> prefix_count_and_prod(int L, int R, Y y) const {
        pair<int, T> ans = {0, Mono::unit()};
        work_prefix([&](int d, int a, int b) {
            ans.first += b - a;
            ans.second = Mono::op(ans.second, seg[d].prod(a, b));
        }, L, R, y);
        return ans;
    }

    // [L, R) x [y1, y2)
    pair<int, T> count_and_prod(int L, int R, Y y1, Y y2) const {
        pair<int, T> ans = {0, Mono::unit()};
        work_range([&](int d, int a, int b) {
            ans.first += b - a;
            ans.second = Mono::op(ans.second, seg[d].prod(a, b));
        }, L, R, y1, y2);
        return ans;
    }

    // k-th smallest (0-indexed) in [L, R)
    Y kth(int L, int R, int k) const {
        assert(0 <= k && k < R - L);
        Y ans = 0;
        for (int d = log - 1; d >= 0; --d) {
            auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
            if (k < R0 - L0) {
                L = L0, R = R0;
            } else {
                ans |= Y(1) << d;
                k -= R0 - L0, L = L1, R = R1;
            }
        }
        return ans;
    }

    // {k-th smallest, product of the k smallest}; k = R - L gives {INF, prod_all}
    // ties are taken in index order
    pair<Y, T> kth_value_and_prod(int L, int R, int k) const {
        assert(0 <= k && k <= R - L);
        if (k == R - L) return {INF, prod_all(L, R)};
        Y ans = 0;
        T t = Mono::unit();
        for (int d = log - 1; d >= 0; --d) {
            auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
            if (k < R0 - L0) {
                L = L0, R = R0;
            } else {
                ans |= Y(1) << d, t = Mono::op(t, seg[d].prod(L0, R0));
                k -= R0 - L0, L = L1, R = R1;
            }
        }
        t = Mono::op(t, seg[0].prod(L, L + k));
        return {ans, t};
    }

    // product of the [k1, k2)-th smallest; needs Mono::inverse
    T prod_index_range(int L, int R, int k1, int k2) const {
        T t1 = kth_value_and_prod(L, R, k1).second;
        T t2 = kth_value_and_prod(L, R, k2).second;
        return Mono::op(Mono::inverse(t1), t2);
    }

    // lower median for upper = false, upper median for upper = true
    template <bool upper>
    Y median(int L, int R) const {
        assert(0 <= L && L < R && R <= n);
        int k = (upper ? (R - L) / 2 : (R - L - 1) / 2);
        return kth(L, R, k);
    }

    // smallest value >= y in [L, R), INF if none
    Y next(int L, int R, Y y) const {
        int c = prefix_count(L, R, y);
        return (c == R - L ? INF : kth(L, R, c));
    }

    // largest value <= y in [L, R), -INF if none
    Y prev(int L, int R, Y y) const {
        int c = (y >= limit ? R - L : prefix_count(L, R, y + 1));
        return (c == 0 ? -INF : kth(L, R, c - 1));
    }

    void set(int i, T t) { work_point([&](int d, int j) { seg[d].set(j, t); }, i); }
    void multiply(int i, T t) { work_point([&](int d, int j) { seg[d].multiply(j, t); }, i); }
    void add(int i, T t) { work_point([&](int d, int j) { seg[d].add(j, t); }, i); }

    // largest y with check(y, cnt, prod) over [L, R) x [0, y); {INF, all} if check holds for everything.
    // check monotone, check(0, 0, unit) true. cnt counts all points: with weights used as
    // on/off switches, look at prod instead.
    template <typename F>
    tuple<Y, int, T> max_right(int L, int R, F check) const {
        return max_right_many({{L, R}}, check);
    }

    // same over the union of the ranges in LR
    template <typename F>
    tuple<Y, int, T> max_right_many(vector<pair<int, int>> LR, F check) const {
        int cnt = 0, cnt_all = 0;
        Y y = 0;
        T t = Mono::unit(), t_all = Mono::unit();
        for (auto& [l, r]: LR) t_all = Mono::op(t_all, prod_all(l, r)), cnt_all += r - l;
        assert(check(0, 0, Mono::unit()));
        if (check(INF, cnt_all, t_all)) return {INF, cnt_all, t_all};
        for (int d = log - 1; d >= 0; --d) {
            Y y1 = y | Y(1) << d;
            T t1 = t;
            int cnt1 = cnt;
            for (auto& [L, R]: LR) {
                auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
                cnt1 += R0 - L0;
                t1 = Mono::op(t1, seg[d].prod(L0, R0));
            }
            bool ok = check(y1, cnt1, t1);
            if (ok) y = y1, cnt = cnt1, t = t1;
            for (auto& [L, R]: LR) {
                auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
                if (ok) L = L1, R = R1;
                else L = L0, R = R0;
            }
        }
        return {y, cnt, t};
    }

    // smallest y with check(y, cnt, prod) over the union of LR x [y, limit); {0, all} if check(0, ...).
    // check monotone, check(limit, 0, unit) true.
    // https://qoj.ac/contest/1047/problem/5094
    template <typename F>
    tuple<Y, int, T> min_left_many(vector<pair<int, int>> LR, F check) const {
        assert(check(limit, 0, Mono::unit()));
        int cnt = 0, cnt_all = 0;
        Y y = limit;
        T t = Mono::unit(), t_all = Mono::unit();
        for (auto& [l, r]: LR) t_all = Mono::op(t_all, prod_all(l, r)), cnt_all += r - l;
        if (check(0, cnt_all, t_all)) return {0, cnt_all, t_all};
        for (int d = log - 1; d >= 0; --d) {
            Y y1 = y - (Y(1) << d);
            T t1 = t;
            int cnt1 = cnt;
            for (auto& [L, R]: LR) {
                auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
                cnt1 += R1 - L1;
                t1 = Mono::op(t1, seg[d].prod(L1, R1));
            }
            bool ok = check(y1, cnt1, t1);
            if (ok) y = y1, cnt = cnt1, t = t1;
            for (auto& [L, R]: LR) {
                auto [L0, R0, L1, R1] = get_subtree(d + 1, L, R);
                if (ok) L = L0, R = R0;
                else L = L1, R = R1;
            }
        }
        return {y, cnt, t};
    }
};

// Any values: compressed to ranks 0..K-1, then an Uncompressed_Wavelet_Matrix<int>.
template <typename Y, typename SEGTREE>
struct Compressed_Wavelet_Matrix {
    using Mono = typename SEGTREE::MX;
    using T = typename Mono::value_type;
    static constexpr Y INF = numeric_limits<Y>::max() / 2;

    int n = 0;
    vector<Y> key; // sorted distinct values
    Uncompressed_Wavelet_Matrix<int, SEGTREE> wm;

    Compressed_Wavelet_Matrix() = default;

    // f(i) = {A[i], dat[i]}
    template <typename F>
    Compressed_Wavelet_Matrix(int n, F f) {
        build(n, f);
    }
    Compressed_Wavelet_Matrix(const vector<Y>& A) { build(A); }

    template <typename F>
    void build(int n, F f) {
        this->n = n;
        vector<Y> A(n);
        vector<T> S(n);
        For(i, n) tie(A[i], S[i]) = f(i);
        key = A;
        settify(key);
        wm.build(n, [&](int i) -> pair<int, T> { return {idx(A[i]), S[i]}; });
    }

    void build(const vector<Y>& A) {
        static_assert(is_same_v<SEGTREE, Dummy_Data_Structure>);
        build(len(A), [&](int i) -> pair<Y, T> { return {A[i], Mono::unit()}; });
    }

    // number of keys < y
    int idx(Y y) const { return lower_bound(key.begin(), key.end(), y) - key.begin(); }

    Y kth(int L, int R, int k) const { return key[wm.kth(L, R, k)]; }

    template <bool upper>
    Y median(int L, int R) const { return key[wm.template median<upper>(L, R)]; }

    // [L, R) x [-inf, y)
    int prefix_count(int L, int R, Y y) const { return wm.prefix_count(L, R, idx(y)); }
    // [L, R) x [y1, y2)
    int count(int L, int R, Y y1, Y y2) const { return wm.count(L, R, idx(y1), idx(y2)); }
    // [L, R) x [-inf, y)
    T prefix_prod(int L, int R, Y y) const { return wm.prefix_prod(L, R, idx(y)); }
    // [L, R) x [y1, y2)
    T prod(int L, int R, Y y1, Y y2) const { return wm.prod(L, R, idx(y1), idx(y2)); }
    T prod_all(int L, int R) const { return wm.prod_all(L, R); }
    // [L, R) x [-inf, y)
    pair<int, T> prefix_count_and_prod(int L, int R, Y y) const {
        return wm.prefix_count_and_prod(L, R, idx(y));
    }
    // [L, R) x [y1, y2)
    pair<int, T> count_and_prod(int L, int R, Y y1, Y y2) const {
        return wm.count_and_prod(L, R, idx(y1), idx(y2));
    }

    pair<Y, T> kth_value_and_prod(int L, int R, int k) const {
        auto [p, t] = wm.kth_value_and_prod(L, R, k);
        return {(k == R - L ? INF : key[p]), t};
    }
    T prod_index_range(int L, int R, int k1, int k2) const { return wm.prod_index_range(L, R, k1, k2); }

    // smallest value >= y in [L, R), INF if none
    Y next(int L, int R, Y y) const {
        int c = prefix_count(L, R, y);
        return (c == R - L ? INF : kth(L, R, c));
    }

    // largest value <= y in [L, R), -INF if none
    Y prev(int L, int R, Y y) const {
        int c = wm.prefix_count(L, R, upper_bound(key.begin(), key.end(), y) - key.begin());
        return (c == 0 ? -INF : kth(L, R, c - 1));
    }

    void set(int i, T t) { wm.set(i, t); }
    void multiply(int i, T t) { wm.multiply(i, t); }
    void add(int i, T t) { wm.add(i, t); }
};

// compress = false: values in [0, 2^log) used as they are (Y = int / long long)
template <typename Y, bool compress, typename SEGTREE = Dummy_Data_Structure>
using Wavelet_Matrix = conditional_t<compress, Compressed_Wavelet_Matrix<Y, SEGTREE>,
                                     Uncompressed_Wavelet_Matrix<Y, SEGTREE>>;
} // namespace mitsuha
#endif // AJAY_WAVELET_MATRIX
