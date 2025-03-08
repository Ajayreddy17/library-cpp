#define AJAY_WAVELET_MATRIX_OLD
#define AJAY_WAVELET_MATRIX_OLD

#include "library/monoid/monoid_add.hpp"
#include "library/datastructure/wavelet_matrix/bit_vector.hpp"

namespace mitsuha{
// Specify whether to apply seating pressure with COMPRESS
// When using xor-like usage, pass log in the constructor
template <typename T, bool COMPRESS, typename Monoid = Monoid_Add<T>>
struct Wavelet_Matrix_Old {
    using MX = Monoid;
    using X = typename MX::value_type;
    static_assert(MX::commute);
    int N, lg;
    vector<int> mid;
    vector<Bit_Vector> bv;
    vector<T> key;
    bool set_log;
    vector<vector<X>> cumsum;

    Wavelet_Matrix_Old() {}
    // If you are not using sum, SUM_data can be empty.
    Wavelet_Matrix_Old(vector<T> A, vector<X> SUM_data = {}, int log = -1) {
        build(A, SUM_data, log);
    }

    void build(vector<T> A, vector<X> SUM_data = {}, int log = -1) {
        N = (int) A.size(), lg = log, set_log = (log != -1);
        bool MAKE_SUM = !(SUM_data.empty());
        vector<X>& S = SUM_data;
        if (COMPRESS) {
            assert(!set_log);
            key.reserve(N);
            auto I = argsort(A);
            for (auto&& i: I) {
                if (key.empty() || key.back() != A[i]) key.emplace_back(A[i]);
                A[i] = (int) key.size() - 1;
            }
            key.shrink_to_fit();
        }
        if (lg == -1) lg = __lg(max<long long>(*max_element(A.begin(), A.end()), 1)) + 1;
        mid.resize(lg);
        bv.assign(lg, Bit_Vector(N));
        if (MAKE_SUM) cumsum.assign(1 + lg, vector<X>(N + 1, MX::unit()));
        S.resize(N);

        vector<T> A0(N), A1(N);
        vector<X> S0(N), S1(N);
        for (int d = lg - 1; d >= -1; --d) {
            int p0 = 0, p1 = 0;
            if (MAKE_SUM) {
                for(int i = 0; i < N; i++) { cumsum[d + 1][i + 1] = MX::op(cumsum[d + 1][i], S[i]); }
            }
            if (d == -1) break;
            for(int i = 0; i < N; i++) {
                bool f = (A[i] >> d & 1);
                if (!f) {
                    if (MAKE_SUM) S0[p0] = S[i];
                    A0[p0++] = A[i];
                }
                if (f) {
                    if (MAKE_SUM) S1[p1] = S[i];
                    bv[d].set(i), A1[p1++] = A[i];
                }
            }
            mid[d] = p0;
            bv[d].build();
            swap(A, A0), swap(S, S0);
            for(int i = 0; i < p1; i++) A[p0 + i] = A1[i], S[p0 + i] = S1[i];
        }
    }

    // Count the xor results that fit in [a, b)
    int count(int L, int R, T a, T b, T xor_val = 0) {
        return prefix_count(L, R, b, xor_val) - prefix_count(L, R, a, xor_val);
    }

    int count(vector<pair<int, int>> segments, T a, T b, T xor_val = 0) {
        int res = 0;
        for (auto&& [L, R]: segments) res += count(L, R, a, b, xor_val);
        return res;
    }

    // xor result, k>=0th and prefix sum in [L, R)
    pair<T, X> kth_value_and_sum(int L, int R, int k, T xor_val = 0) {
        assert(!cumsum.empty());
        if (xor_val != 0) assert(set_log);
        assert(0 <= k && k <= R - L);
        if (k == R - L) { return {numeric_limits<T>::max() / 2, sum_all(L, R)}; }
        int cnt = 0;
        X sm = MX::unit();
        T ret = 0;
        for (int d = lg - 1; d >= 0; --d) {
            bool f = (xor_val >> d) & 1;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int c = (f ? (R - L) - (r0 - l0) : (r0 - l0));
            if (cnt + c > k) {
                if (!f) L = l0, R = r0;
                if (f) L += mid[d] - l0, R += mid[d] - r0;
            } else {
                X s = (f ? get(d, L + mid[d] - l0, R + mid[d] - r0) : get(d, l0, r0));
                cnt += c, ret |= T(1) << d, sm = MX::op(sm, s);
                if (!f) L += mid[d] - l0, R += mid[d] - r0;
                if (f) L = l0, R = r0;
            }
        }
        sm = MX::op(sm, get(0, L, L + k - cnt));
        if (COMPRESS) ret = key[ret];
        return {ret, sm};
    }

    // xor result, k>=0th in [L, R) and prefix sum
    pair<T, X> kth_value_and_sum(vector<pair<int, int>> segments, int k, T xor_val = 0) {
        assert(!cumsum.empty());
        if (xor_val != 0) assert(set_log);
        int total_len = 0;
        for (auto&& [L, R]: segments) total_len += R - L;
        assert(0 <= k && k <= total_len);
        if (k == total_len) { return {numeric_limits<T>::max() / 2, sum_all(segments)}; }
        int cnt = 0;
        X sm = MX::unit();
        T ret = 0;
        for (int d = lg - 1; d >= 0; --d) {
            bool f = (xor_val >> d) & 1;
            int c = 0;
            for (auto&& [L, R]: segments) {
                int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
                c += (f ? (R - L) - (r0 - l0) : (r0 - l0));
            }
            if (cnt + c > k) {
                for (auto&& [L, R]: segments) {
                    int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
                    if (!f) L = l0, R = r0;
                    if (f) L += mid[d] - l0, R += mid[d] - r0;
                }
            } else {
                cnt += c, ret |= T(1) << d;
                for (auto&& [L, R]: segments) {
                    int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
                    X s = (f ? get(d, L + mid[d] - l0, R + mid[d] - r0) : get(d, l0, r0));
                    sm = MX::op(sm, s);
                    if (!f) L += mid[d] - l0, R += mid[d] - r0;
                    if (f) L = l0, R = r0;
                }
            }
        }
        for (auto&& [L, R]: segments) {
            int t = min(R - L, k - cnt);
            sm = MX::op(sm, get(0, L, L + t));
            cnt += t;
        }
        if (COMPRESS) ret = key[ret];
        return {ret, sm};
    }

    // xor result, k>=0th in [L, R)
    T kth(int L, int R, int k, T xor_val = 0) {
        if (xor_val != 0) assert(set_log);
        assert(0 <= k && k < R - L);
        int cnt = 0;
        T ret = 0;
        for (int d = lg - 1; d >= 0; --d) {
            bool f = (xor_val >> d) & 1;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int c = (f ? (R - L) - (r0 - l0) : (r0 - l0));
            if (cnt + c > k) {
                if (!f) L = l0, R = r0;
                if (f) L += mid[d] - l0, R += mid[d] - r0;
            } else {
                cnt += c, ret |= T(1) << d;
                if (!f) L += mid[d] - l0, R += mid[d] - r0;
                if (f) L = l0, R = r0;
            }
        }
        if (COMPRESS) ret = key[ret];
        return ret;
    }

    T kth(vector<pair<int, int>> segments, int k, T xor_val = 0) {
        int total_len = 0;
        for (auto&& [L, R]: segments) total_len += R - L;
        assert(0 <= k && k < total_len);
        int cnt = 0;
        T ret = 0;
        for (int d = lg - 1; d >= 0; --d) {
            bool f = (xor_val >> d) & 1;
            int c = 0;
            for (auto&& [L, R]: segments) {
                int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
                c += (f ? (R - L) - (r0 - l0) : (r0 - l0));
            }
            if (cnt + c > k) {
                for (auto&& [L, R]: segments) {
                    int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
                    if (!f) L = l0, R = r0;
                    if (f) L += mid[d] - l0, R += mid[d] - r0;
                }
            } else {
                cnt += c, ret |= T(1) << d;
                for (auto&& [L, R]: segments) {
                    int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
                    if (!f) L += mid[d] - l0, R += mid[d] - r0;
                    if (f) L = l0, R = r0;
                }
            }
        }
        if (COMPRESS) ret = key[ret];
        return ret;
    }

    // The median value in [L, R) as a result of xoring.
    // LOWER = true: lower median, false: upper median
    T median(bool UPPER, int L, int R, T xor_val = 0) {
        int n = R - L;
        int k = (UPPER ? n / 2 : (n - 1) / 2);
        return kth(L, R, k, xor_val);
    }

    T median(bool UPPER, vector<pair<int, int>> segments, T xor_val = 0) {
        int n = 0;
        for (auto&& [L, R]: segments) n += R - L;
        int k = (UPPER ? n / 2 : (n - 1) / 2);
        return kth(segments, k, xor_val);
    }

    // Sum of SUM_data at the [k1, k2)th point in the xor result
    X sum(int L, int R, int k1, int k2, T xor_val = 0) {
        X add = prefix_sum(L, R, k2, xor_val);
        X sub = prefix_sum(L, R, k1, xor_val);
        return MX::op(add, MX::inverse(sub));
    }

    X sum_all(int L, int R) { return get(lg, L, R); }

    X sum_all(vector<pair<int, int>> segments) {
        X sm = MX::unit();
        for (auto&& [L, R]: segments) { sm = MX::op(sm, get(lg, L, R)); }
        return sm;
    }

    // The largest (cnt, sum) such that check(cnt, prefix sum) is true
    template <typename F>
    pair<int, X> max_right(F check, int L, int R, T xor_val = 0) {
        assert(check(0, MX::unit()));
        if (xor_val != 0) assert(set_log);
        if (check(R - L, get(lg, L, R))) return {R - L, get(lg, L, R)};
        int cnt = 0;
        X sm = MX::unit();
        for (int d = lg - 1; d >= 0; --d) {
            bool f = (xor_val >> d) & 1;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int c = (f ? (R - L) - (r0 - l0) : (r0 - l0));
            X s = (f ? get(d, L + mid[d] - l0, R + mid[d] - r0) : get(d, l0, r0));
            if (check(cnt + c, MX::op(sm, s))) {
                cnt += c, sm = MX::op(sm, s);
                if (f) L = l0, R = r0;
                if (!f) L += mid[d] - l0, R += mid[d] - r0;
            } else {
                if (!f) L = l0, R = r0;
                if (f) L += mid[d] - l0, R += mid[d] - r0;
            }
        }
        int k = binary_search( [&](int k) -> bool { return check(cnt + k, MX::op(sm, get(0, L, L + k))); }, 0, R - L);
        cnt += k;
        sm = MX::op(sm, get(0, L, L + k));
        return {cnt, sm};
    }

private:
    template <typename F>
    long long binary_search(F check, long long ok, long long ng, bool check_ok = true) {
        if (check_ok) assert(check(ok));
        while (abs(ok - ng) > 1) {
            auto x = (ng + ok) / 2;
            tie(ok, ng) = (check(x) ? make_pair(x, ng) : make_pair(ok, x));
        }
        return ok;
    }

    inline X get(int d, int L, int R) {
        assert(!cumsum.empty());
        return MX::op(MX::inverse(cumsum[d][L]), cumsum[d][R]);
    }

    int prefix_count(int L, int R, T x, T xor_val = 0) {
        if (xor_val != 0) assert(set_log);
        x = (COMPRESS ? (lower_bound(key.begin(), key.end(), x) - key.begin()) : x);
        if (x == 0) return 0;
        if (x >= (1 << lg)) return R - L;
        int cnt = 0;
        for(int d = lg - 1; d >= 0; d--) {
            bool add = (x >> d) & 1;
            bool f = ((xor_val) >> d) & 1;
            int l0 = bv[d].count(L, 0), r0 = bv[d].count(R, 0);
            int kf = (f ? (R - L) - (r0 - l0) : (r0 - l0));
            if (add) {
                cnt += kf;
                if (f) { L = l0, R = r0; }
                if (!f) { L += mid[d] - l0, R += mid[d] - r0; }
            } else {
                if (!f) L = l0, R = r0;
                if (f) L += mid[d] - l0, R += mid[d] - r0;
            }
        }
        return cnt;
    }

    X prefix_sum(int L, int R, int k, T xor_val = 0) {
        return kth_value_and_sum(L, R, k, xor_val).second;
    }

    X prefix_sum(vector<pair<int, int>> segments, int k, T xor_val = 0) {
        return kth_value_and_sum(segments, k, xor_val).second;
    }
};
} // namespace mitsuha