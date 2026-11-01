#ifndef AJAY_MASPY_COMPAT
#define AJAY_MASPY_COMPAT

// Helpers from maspypy's my_template.hpp that the base template does not have.
#define U32(...) unsigned int __VA_ARGS__; read(__VA_ARGS__)
#define U64(...) unsigned long long __VA_ARGS__; read(__VA_ARGS__)
// Included by files ported from https://github.com/maspypy/library.
#include "library/util/pop.hpp"

namespace mitsuha{
template <class T, class S, std::enable_if_t<!std::is_same_v<T, S>, int> = 0>
bool chmax(T &a, const S &b) { return (T(b) > a ? (a = T(b), true) : false); }
template <class T, class S, std::enable_if_t<!std::is_same_v<T, S>, int> = 0>
bool chmin(T &a, const S &b) { return (T(b) < a ? (a = T(b), true) : false); }

inline int popcnt(int x) { return __builtin_popcount(x); }
inline int popcnt(unsigned int x) { return __builtin_popcount(x); }
inline int popcnt(long long x) { return __builtin_popcountll(x); }
inline int popcnt(unsigned long long x) { return __builtin_popcountll(x); }
inline int popcnt(unsigned long x) { return __builtin_popcountll(x); }
inline int popcnt_sgn(int x) { return (__builtin_parity(unsigned(x)) & 1 ? -1 : 1); }
inline int popcnt_sgn(unsigned int x) { return (__builtin_parity(x) & 1 ? -1 : 1); }
inline int popcnt_sgn(long long x) { return (__builtin_parityll(x) & 1 ? -1 : 1); }
inline int popcnt_sgn(unsigned long long x) { return (__builtin_parityll(x) & 1 ? -1 : 1); }

// (0, 1, 2, 3, 4) -> (-1, 0, 1, 1, 2)
inline int topbit(int x) { return (x == 0 ? -1 : 31 - __builtin_clz(x)); }
inline int topbit(unsigned int x) { return (x == 0 ? -1 : 31 - __builtin_clz(x)); }
inline int topbit(long long x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
inline int topbit(unsigned long long x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
inline int topbit(unsigned long x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 0, 2)
inline int lowbit(int x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
inline int lowbit(unsigned int x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
inline int lowbit(long long x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
inline int lowbit(unsigned long long x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
inline int lowbit(unsigned long x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }

template <class T, class Container, class Compare,
          std::enable_if_t<!std::is_same_v<Compare, std::less<T>> && !std::is_same_v<Compare, std::greater<T>>, int> = 0>
T POP(std::priority_queue<T, Container, Compare> &que) {
    T a = que.top();
    que.pop();
    return a;
}

template <typename T>
T kth_bit(int k) {
    assert(0 <= k && k < int(8 * sizeof(T)));
    return T(1) << k;
}
template <typename T>
bool has_kth_bit(T x, int k) {
    assert(0 <= k && k < int(8 * sizeof(T)));
    return x >> k & 1;
}

// for (int i : all_bit(s)): indices of set bits, ascending
template <typename T>
struct all_bit {
    static_assert(std::is_integral<T>::value);
    T s;
    all_bit(T s) : s(s) { assert(s >= 0); }
    struct iter {
        T s;
        int operator*() const { return lowbit(s); }
        void operator++() { s &= s - 1; }
        bool operator!=(std::nullptr_t) const { return s; }
    };
    iter begin() const { return {s}; }
    std::nullptr_t end() const { return nullptr; }
};

// for (T t : all_subset(s)): all submasks of s, descending, including 0
template <typename T>
struct all_subset {
    static_assert(std::is_integral<T>::value);
    T s;
    all_subset(T s) : s(s) { assert(s >= 0); }
    struct iter {
        T s, t;
        bool done = false;
        T operator*() const { return t; }
        void operator++() {
            done = (t == 0);
            t = (t - 1) & s;
        }
        bool operator!=(std::nullptr_t) const { return !done; }
    };
    iter begin() const { return {s, s}; }
    std::nullptr_t end() const { return nullptr; }
};

constexpr uint64_t full_mask(int n) {
    assert(0 <= n && n <= 64);
    return n == 64 ? -1ULL : (1ULL << n) - 1;
}

inline uint64_t bit_reverse(uint64_t x) {
    x = ((x & 0x5555555555555555ULL) << 1) | ((x >> 1) & 0x5555555555555555ULL);
    x = ((x & 0x3333333333333333ULL) << 2) | ((x >> 2) & 0x3333333333333333ULL);
    x = ((x & 0x0f0f0f0f0f0f0f0fULL) << 4) | ((x >> 4) & 0x0f0f0f0f0f0f0f0fULL);
    x = ((x & 0x00ff00ff00ff00ffULL) << 8) | ((x >> 8) & 0x00ff00ff00ff00ffULL);
    x = ((x & 0x0000ffff0000ffffULL) << 16) | ((x >> 16) & 0x0000ffff0000ffffULL);
    return (x << 32) | (x >> 32);
}

// characters -> S[i] - first_char; '?' -> -1
inline std::vector<int> s_to_vi(const std::string &S, char first_char) {
    std::vector<int> A(S.size());
    for (size_t i = 0; i < S.size(); ++i) A[i] = (S[i] != '?' ? S[i] - first_char : -1);
    return A;
}

// x mod y in [0, y), requires y > 0
template <typename T>
T bmod(T x, T y) {
    T r = x % y;
    return (r < 0 ? r + y : r);
}

template <typename T, typename U>
T SUM(const U &A) { return std::accumulate(A.begin(), A.end(), T{}); }

template <class C, class T>
inline long long LB(const C &c, const T &x) { return std::lower_bound(c.begin(), c.end(), x) - c.begin(); }
template <class C, class T>
inline long long UB(const C &c, const T &x) { return std::upper_bound(c.begin(), c.end(), x) - c.begin(); }

// returns {0, A[0], A[0]+A[1], ...}; off = 0 drops the leading 0
template <typename T, typename U>
std::vector<T> cumsum(const std::vector<U> &A, int off = 1) {
    int N = A.size();
    std::vector<T> B(N + 1);
    for (int i = 0; i < N; ++i) B[i + 1] = B[i] + A[i];
    if (off == 0) B.erase(B.begin());
    return B;
}

// check(ok) is true, check(ng) is false; returns the ok side of the boundary
template <typename F>
__int128 binary_search_check(F check, __int128 ok, __int128 ng, bool check_ok = true) {
    if (check_ok) assert(check(ok));
    while (1) {
        __int128 x = (ok + ng) / 2;
        if (x == ok || x == ng) break;
        (check(x) ? ok : ng) = x;
    }
    return ok;
}

template <typename F>
double binary_search_real(F check, double ok, double ng, int iter = 100) {
    for (int i = 0; i < iter; ++i) {
        double x = (ok + ng) / 2;
        (check(x) ? ok : ng) = x;
    }
    return (ok + ng) / 2;
}
} // namespace mitsuha
#endif // AJAY_MASPY_COMPAT
