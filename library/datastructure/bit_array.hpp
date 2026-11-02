#ifndef AJAY_BIT_ARRAY
#define AJAY_BIT_ARRAY

#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/ds/bit_array.hpp
/*
Manages a 0/1 sequence, stored internally in 64-bit words. Intended uses:
- a variable-length bitset that supports slicing
- polynomials over F_2
and so on.
*/
struct Bit_Array {
    using T = Bit_Array;
    int N;
    vector<uint64_t> dat;

    // fill with x
    Bit_Array(int N = 0, int x = 0) : N(N) {
        assert(0 <= N && N <= INT_MAX - 63);
        assert(x == 0 || x == 1);
        uint64_t v = (x == 0 ? 0 : -1);
        dat.assign((N + 63) >> 6, v);
        resize(N);
    }

    int size() const { return N; }

    void resize(int n) {
        assert(0 <= n && n <= INT_MAX - 63);
        dat.resize((n + 63) >> 6);
        int r = n & 63;
        if (r) dat.back() &= (1ULL << r) - 1;
        N = n;
    }

    void fill0() { fill(dat.begin(), dat.end(), uint64_t(0)); }
    void fill1() {
        fill(dat.begin(), dat.end(), uint64_t(-1));
        resize(N);
    }

    void push_back(bool b) {
        resize(N + 1);
        (*this)[N - 1] = b;
    }

    static T from_string(const string &S) {
        int N = len(S);
        T ANS(N);
        For(i, N) ANS[i] = (S[i] == '1');
        return ANS;
    }

    class Proxy {
      public:
        Proxy(vector<uint64_t> &d, int i) : dat(d), index(i) {}
        operator bool() const { return (dat[index >> 6] >> (index & 63)) & 1; }
        Proxy &operator=(bool value) {
            uint64_t mask = uint64_t(1) << (index & 63);
            if (value)
                dat[index >> 6] |= mask;
            else
                dat[index >> 6] &= ~mask;
            return *this;
        }
        Proxy &operator=(const Proxy &p) { return *this = bool(p); }

        // bit operations
        Proxy &operator^=(bool x) {
            if (x) dat[index >> 6] ^= uint64_t(1) << (index & 63);
            return *this;
        }
        Proxy &operator|=(bool x) {
            if (x) dat[index >> 6] |= uint64_t(1) << (index & 63);
            return *this;
        }
        Proxy &operator&=(bool x) {
            if (!x) dat[index >> 6] &= ~(uint64_t(1) << (index & 63));
            return *this;
        }
        // finite field F_2
        Proxy &operator+=(bool x) { return *this ^= x; }
        Proxy &operator-=(bool x) { return *this ^= x; }
        Proxy &operator*=(bool x) { return *this &= x; }
        Proxy &operator/=(bool x) {
            assert(x);
            return *this;
        }
        bool inverse() const {
            assert(bool(*this));
            return true;
        }
        void flip() { *this ^= true; }

      private:
        vector<uint64_t> &dat;
        int index;
    };

    Proxy operator[](int i) {
        assert(0 <= i && i < N);
        return Proxy(dat, i);
    }
    bool operator[](int i) const {
        assert(0 <= i && i < N);
        return (dat[i >> 6] >> (i & 63)) & 1;
    }

    bool operator==(const T &p) const {
        if (N != p.N) return false;
        For(i, len(dat)) if (dat[i] != p.dat[i]) return false;
        return true;
    }

    T &operator&=(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) dat[i] &= p.dat[i];
        return *this;
    }
    T &operator|=(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) dat[i] |= p.dat[i];
        return *this;
    }
    T &operator^=(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) dat[i] ^= p.dat[i];
        return *this;
    }
    T operator&(const T &p) const { return T(*this) &= p; }
    T operator|(const T &p) const { return T(*this) |= p; }
    T operator^(const T &p) const { return T(*this) ^= p; }
    T operator~() const {
        T p = (*this);
        p.flip_range(0, N);
        return p;
    }

    void set_minus_inplace(const T &other) {
        assert(N == other.N);
        For(i, len(dat)) dat[i] = dat[i] & (~other.dat[i]);
    }

    T set_minus(T other) const {
        assert(N == other.N);
        For(i, len(dat)) other.dat[i] = dat[i] & ~other.dat[i];
        return other;
    }

    int count() const {
        int ans = 0;
        for (uint64_t val : dat) ans += popcount(val);
        return ans;
    }

    // size of set intersection, not modulo 2
    int dot(const T &p) const {
        assert(N == p.N);
        int ans = 0;
        For(i, len(dat)) ans += popcount(dat[i] & p.dat[i]);
        return ans;
    }

    // minimum j >= i with (*this)[j] = 1, or N if none
    int next(int i) const {
        chmax(i, 0);
        if (i >= N) return N;
        int k = i >> 6;
        {
            uint64_t x = dat[k];
            int s = i & 63;
            x = (x >> s) << s;
            if (x) return (k << 6) | lsb(x);
        }
        For(idx, k + 1, len(dat)) {
            if (dat[idx] == 0) continue;
            return (idx << 6) | lsb(dat[idx]);
        }
        return N;
    }

    // maximum j <= i with (*this)[j] = 1, or -1 if none
    int prev(int i) const {
        chmin(i, N - 1);
        if (i <= -1) return -1;
        int k = i >> 6;
        if ((i & 63) < 63) {
            uint64_t x = dat[k];
            x &= (uint64_t(1) << ((i & 63) + 1)) - 1;
            if (x) return (k << 6) | msb(x);
            --k;
        }
        Frr(idx, k + 1) {
            if (dat[idx] == 0) continue;
            return (idx << 6) | msb(dat[idx]);
        }
        return -1;
    }

    Bit_Array slice(int L, int R) const {
        assert(0 <= L && L <= R && R <= N);
        Bit_Array p(R - L);
        int rm = (R - L) & 63;
        For(rm) {
            int j = R - L - 1;
            p.dat[j >> 6] |= uint64_t((dat[(R - 1) >> 6] >> ((R - 1) & 63)) & 1)
                                              << (j & 63);
            --R;
        }
        int n = (R - L) >> 6;
        int hi = L & 63;
        int lo = 64 - hi;
        int s = L >> 6;
        if (hi == 0) {
            For(i, n) { p.dat[i] = dat[s + i]; }
        } else {
            For(i, n) { p.dat[i] = (dat[s + i] >> hi) | (dat[s + i + 1] << lo); }
        }
        return p;
    }

    int count_range(int L, int R) const {
        assert(0 <= L && L <= R && R <= N);
        int cnt = 0;
        while ((L < R) && (L & 63)) cnt += (*this)[L++];
        while ((L < R) && (R & 63)) cnt += (*this)[--R];
        int l = L >> 6, r = R >> 6;
        For(i, l, r) cnt += popcount(dat[i]);
        return cnt;
    }

    /*
    OP = 0: assign, 1: or, 2: xor, 3: and.
    Only a self-referencing, overlapping range is copied aside. When ranges do not overlap, input and output
    are used directly, keeping the word loop simple in the common case.
    */
    template <int OP>
    void apply_to_range(
            int L, int R, const Bit_Array &other, int L1 = -1, int R1 = -1) {
        static_assert(0 <= OP && OP < 4);
        assert(0 <= L && L <= R && R <= N);
        if (L1 == -1) L1 = 0, R1 = other.N;
        assert(0 <= L1 && L1 <= R1 && R1 <= other.N);
        assert(R - L == R1 - L1);

        if (&other == this && max(L, L1) < min(R, R1)) {
            Bit_Array tmp = other.slice(L1, R1);
            apply_to_range<OP>(L, R, tmp);
            return;
        }

        auto apply_bit = [&](int i, bool x) {
            uint64_t bit = uint64_t(x) << (i & 63);
            if constexpr (OP == 0)
                dat[i >> 6] = (dat[i >> 6] & ~(uint64_t(1) << (i & 63))) | bit;
            if constexpr (OP == 1) dat[i >> 6] |= bit;
            if constexpr (OP == 2) dat[i >> 6] ^= bit;
            if constexpr (OP == 3) dat[i >> 6] &= ~(uint64_t(!x) << (i & 63));
        };
        auto apply_word = [&](int i, uint64_t x) {
            if constexpr (OP == 0) dat[i] = x;
            if constexpr (OP == 1) dat[i] |= x;
            if constexpr (OP == 2) dat[i] ^= x;
            if constexpr (OP == 3) dat[i] &= x;
        };
        auto get_bit = [&](int i) -> bool {
            return (other.dat[i >> 6] >> (i & 63)) & 1;
        };

        while (L < R && (L & 63)) apply_bit(L++, get_bit(L1++));
        int n = (R - L) >> 6;
        int l = L >> 6, s = L1 >> 6, sh = L1 & 63;
        if (sh == 0) {
            For(i, n) apply_word(l + i, other.dat[s + i]);
        } else {
            int lo = 64 - sh;
            For(i, n) {
                apply_word(
                        l + i, (other.dat[s + i] >> sh) | (other.dat[s + i + 1] << lo));
            }
        }
        L += 64 * n, L1 += 64 * n;
        while (L < R) apply_bit(L++, get_bit(L1++));
    }

    void assign_to_range(
            int L, int R, const Bit_Array &other, int L1 = -1, int R1 = -1) {
        apply_to_range<0>(L, R, other, L1, R1);
    }
    void or_to_range(
            int L, int R, const Bit_Array &other, int L1 = -1, int R1 = -1) {
        apply_to_range<1>(L, R, other, L1, R1);
    }
    void xor_to_range(
            int L, int R, const Bit_Array &other, int L1 = -1, int R1 = -1) {
        apply_to_range<2>(L, R, other, L1, R1);
    }
    void and_to_range(
            int L, int R, const Bit_Array &other, int L1 = -1, int R1 = -1) {
        apply_to_range<3>(L, R, other, L1, R1);
    }

    // xor p into this, assuming p is nonzero only on [i, N)
    // useful for elementary row operations
    void xor_suffix(int i, const Bit_Array &p) {
        assert(N == p.N && 0 <= i && i < N);
        For(k, i / 64, len(dat)) { dat[k] ^= p.dat[k]; }
    }

    // set [L,R) to 1
    void set_range(int L, int R) {
        assert(0 <= L && L <= R && R <= N);
        while (L < R && (L & 63)) set(L++);
        while (L < R && (R & 63)) set(--R);
        For(i, L >> 6, R >> 6) dat[i] = uint64_t(-1);
    }

    // set [L,R) to 0
    void reset_range(int L, int R) {
        assert(0 <= L && L <= R && R <= N);
        while (L < R && (L & 63)) reset(L++);
        while (L < R && (R & 63)) reset(--R);
        For(i, L >> 6, R >> 6) dat[i] = uint64_t(0);
    }

    // flip [L,R)
    void flip_range(int L, int R) {
        assert(0 <= L && L <= R && R <= N);
        while (L < R && (L & 63)) flip(L++);
        while (L < R && (R & 63)) flip(--R);
        For(i, L >> 6, R >> 6) dat[i] ^= uint64_t(-1);
    }

    // same interface as std::bitset
    void set(int i) { (*this)[i] = 1; }
    void reset(int i) { (*this)[i] = 0; }
    void flip(int i) { (*this)[i].flip(); }
    void set() { set_range(0, N); }
    void reset() { reset_range(0, N); }
    void flip() { flip_range(0, N); }
    bool any() const {
        For(i, len(dat)) {
            if (dat[i]) return true;
        }
        return false;
    }

    bool has_intersection(const T &other) const {
        assert(N == other.N);
        For(i, len(dat)) if (dat[i] & other.dat[i]) return true;
        return false;
    }

    template <typename F>
    void enumerate_intersection(const T &other, F f) const {
        assert(N == other.N);
        bool end = false;
        For(i, len(dat)) {
            uint64_t x = dat[i] & other.dat[i];
            while (x) {
                int k = lsb(x);
                f(64 * i + k, end);
                if (end) return;
                x &= x - 1;
            }
        }
    }

    bool ALL() const {
        int r = N & 63;
        if (r != 0 && dat.back() != (1ULL << r) - 1) return 0;
        for (int i = 0; i < N / 64; ++i)
            if (dat[i] != uint64_t(-1)) return false;
        return true;
    }

    Bit_Array reversed() const {
        int M = cld(N, 64) * 64;
        Bit_Array a = *this;
        a.resize(M);
        reverse(a.dat.begin(), a.dat.end());
        for (uint64_t &x : a.dat) x = bit_reverse(x);
        return a.slice(M - N, M);
    }

    // all i with bs[i] == true
    vector<int> collect_idx() const {
        vector<int> I;
        For(i, N) if ((*this)[i]) I.emplace_back(i);
        return I;
    }

    bool is_subset(const T &other) const {
        assert(other.N == N);
        For(i, len(dat)) {
            uint64_t a = dat[i], b = other.dat[i];
            if ((a & b) != a) return false;
        }
        return true;
    }

    int _Find_first() const { return next(0); }
    int _Find_next(int p) const { return next(p + 1); }

    template <typename F>
    void enumerate(int L, int R, F f) const {
        assert(0 <= L && L <= R && R <= N);
        if (L == R) return;
        int p = ((*this)[L] ? L : _Find_next(L));
        while (p < R) {
            f(p);
            p = _Find_next(p);
        }
    }

    inline static string TO_STR[256];
    string to_string() const {
        if (TO_STR[0].empty()) precompute();
        string S;
        for (uint64_t x : dat) {
            For(i, 8) S += TO_STR[(x >> (8 * i) & 255)];
        }
        S.resize(N);
        return S;
    }

    static void precompute() {
        For(s, 256) {
            string x;
            For(i, 8) x += '0' + (s >> i & 1);
            TO_STR[s] = x;
        }
    }

    void prefix_xor_sum() {
        int carry = 0;
        for (uint64_t &a : dat) {
            a ^= carry;
            carry = __builtin_parityll(a);
            a ^= a << (1 << 0);
            a ^= a << (1 << 1);
            a ^= a << (1 << 2);
            a ^= a << (1 << 3);
            a ^= a << (1 << 4);
            a ^= a << (1 << 5);
        }
        resize(N);
    }
};
} // namespace mitsuha
#endif // AJAY_BIT_ARRAY
