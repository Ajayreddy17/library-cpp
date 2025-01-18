#ifndef AJAY_INTERVALS
#define AJAY_INTERVALS

#include "library/datastructure/fastset.hpp"

namespace mitsuha {
template <typename T>
struct Intervals_Fast {
    const int LLIM, RLIM;
    const T none_val;
    int total_num;
    int total_len;
    vector<T> dat;
    FastSet ss;

    Intervals_Fast(int N, T none_val)
        : LLIM(0), RLIM(N), none_val(none_val), total_num(0), total_len(0),
          dat(N, none_val), ss(N) {
        ss.insert(0);
    }

    // get (l, r, t) of interval containing x
    tuple<int, int, T> get(int x, bool ERASE = false) {
        int l = ss.prev(x);
        int r = ss.next(x + 1);
        T t = dat[l];
        if (t != none_val && ERASE) {
            --total_num, total_len -= r - l;
            dat[l] = none_val;
            merge_at(l);
            merge_at(r);
        }
        return {l, r, t};
    }

    // [L, R) information in this interval
    // f(l, r, x)
    template <typename F>
    void enumerate_range(int L, int R, F f, bool ERASE = false) {
        assert(LLIM <= L && L <= R && R <= RLIM);
        if (L == R) return;
        if (!ERASE) {
            int l = ss.prev(L);
            while (l < R) {
                int r = ss.next(l + 1);
                f(max(l, L), min(r, R), dat[l]);
                l = r;
            }
            return;
        }
        int p = ss.prev(L);
        if (p < L) {
            ss.insert(L);
            dat[L] = dat[p];
            if (dat[L] != none_val) ++total_num;
        }
        p = ss.next(R);
        if (R < p) {
            dat[R] = dat[ss.prev(R)];
            ss.insert(R);
            if (dat[R] != none_val) ++total_num;
        }
        p = L;
        while (p < R) {
            int q = ss.next(p + 1);
            T x = dat[p];
            f(p, q, x);
            if (dat[p] != none_val) --total_num, total_len -= q - p;
            ss.erase(p);
            p = q;
        }
        ss.insert(L);
        dat[L] = none_val;
    }

    void set(int L, int R, T t) {
        if (L == R) return;
        enumerate_range(L, R, [](int l, int r, T x) -> void {}, true);
        ss.insert(L);
        dat[L] = t;
        if (t != none_val) total_num++, total_len += R - L;
        merge_at(L);
        merge_at(R);
    }

    template <typename F>
    void enumerate_all(F f) {
        enumerate_range(0, RLIM, f, false);
    }

    void merge_at(int p) {
        if (p <= 0 || RLIM <= p) return;
        int q = ss.prev(p - 1);
        if (dat[p] == dat[q]) {
            if (dat[p] != none_val) --total_num;
            ss.erase(p);
        }
    }
};

// https://codeforces.com/contest/1638/problem/E
// value_type = T, coord_type = X
// specify none_val in constructor
template <typename T, typename X = long long>
struct Intervals {
    static constexpr X LLIM = -numeric_limits<X>::max() / 2;
    static constexpr X RLIM = numeric_limits<X>::max() / 2;
    T none_val;
    int total_num;
    X total_len;
    map<X, T> dat;

    Intervals(T none_val) : none_val(none_val), total_num(0), total_len(0) {
        dat[LLIM] = none_val;
        dat[RLIM] = none_val;
    }

    // get (l, r, t) of interval containing x
    tuple<X, X, T> get(X x, bool ERASE = false) {
        auto it2 = dat.upper_bound(x);
        auto it1 = prev(it2);
        auto [l, tl] = *it1;
        auto [r, tr] = *it2;
        if (tl != none_val && ERASE) {
            --total_num, total_len -= r - l;
            dat[l] = none_val;
            merge_at(l);
            merge_at(r);
        }
        return {l, r, tl};
    }

    // [L, R) information in this interval
    // f(l, r, x)
    template <typename F>
    void enumerate_range(X L, X R, F f, bool ERASE = false) {
        assert(LLIM <= L && L <= R && R <= RLIM);
        if (!ERASE) {
            auto it = prev(dat.upper_bound(L));
            while ((*it).first < R) {
                auto it2 = next(it);
                f(max((*it).first, L), min((*it2).first, R), (*it).second);
                it = it2;
            }
            return;
        }
        auto p = prev(dat.upper_bound(L));
        if ((*p).first < L) {
            dat[L] = (*p).second;
            if (dat[L] != none_val) ++total_num;
        }
        p = dat.lower_bound(R);
        if (R < (*p).first) {
            T t = (*prev(p)).second;
            dat[R] = t;
            if (t != none_val) ++total_num;
        }
        p = dat.lower_bound(L);
        while (1) {
            if ((*p).first >= R) break;
            auto q = next(p);
            T t = (*p).second;
            f((*p).first, (*q).first, t);
            if (t != none_val) --total_num, total_len -= (*q).first - (*p).first;
            p = dat.erase(p);
        }
        dat[L] = none_val;
    }

    void set(X L, X R, T t) {
        assert(L <= R);
        if (L == R) return;
        enumerate_range(L, R, [](int l, int r, T x) -> void {}, true);
        dat[L] = t;
        if (t != none_val) total_num++, total_len += R - L;
        merge_at(L);
        merge_at(R);
    }

    template <typename F>
    void enumerate_all(F f, bool ERASE = false) {
        enumerate_range(LLIM, RLIM, f, ERASE);
    }

    void merge_at(X p) {
        if (p == LLIM || RLIM == p) return;
        auto itp = dat.lower_bound(p);
        assert((*itp).first == p);
        auto itq = prev(itp);
        if ((*itp).second == (*itq).second) {
            if ((*itp).second != none_val) --total_num;
            dat.erase(itp);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_INTERVALS