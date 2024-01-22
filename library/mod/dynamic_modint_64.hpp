#ifndef AJAY_DYNAMIC_MODINT_64
#define AJAY_DYNAMIC_MODINT_64

#include "library/mod/primitive_root.hpp"
#include "library/mod/barrett.hpp"

namespace mitsuha{
// https://codeforces.com/contest/453/problem/D
template <int id>
struct Dynamic_Modint_64 {
    static constexpr bool is_modint = true;
    using mint = Dynamic_Modint_64;
    unsigned long long val;
    static Barrett_64 bt;
    static unsigned long long umod() { return bt.umod(); }

    static long long get_mod() { return (long long)(bt.umod()); }
    static void set_mod(long long m) {
        assert(1 <= m);
        bt = Barrett_64(m);
    }

    Dynamic_Modint_64() : val(0) {}
    Dynamic_Modint_64(unsigned long long x) : val(bt.modulo(x)) {}
    Dynamic_Modint_64(unsigned __int128 x) : val(bt.modulo(x)) {}
    Dynamic_Modint_64(int x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}
    Dynamic_Modint_64(long long x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}
    Dynamic_Modint_64(__int128 x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}

    mint& operator+=(const mint& rhs) {
        val = (val += rhs.val) < umod() ? val : val - umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        val = (val += umod() - rhs.val) < umod() ? val : val - umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        val = bt.mul(val, rhs.val);
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inverse(); }
    mint operator-() const { return mint() - *this; }
    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1ULL;
        while (n) {
            if (n & 1) r *= x;
            x *= x, n >>= 1;
        }
        return r;
    }
    mint inverse() const {
        long long x = val, mod = get_mod();
        long long a = x, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        if (u < 0) u += mod;
        return (unsigned long long)(u);
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs.val != rhs.val;
    }
};

template <int id>
void rd(Dynamic_Modint_64<id>& x) {
  io::rd(x.val);
  assert(0 <= x.val && x.val < Dynamic_Modint_64<id>::get_mod());
}
template <int id>
void wt(Dynamic_Modint_64<id> x) {
  io::wt(x.val);
}

using dmint64 = Dynamic_Modint_64<-1>;
template <int id>
Barrett_64 Dynamic_Modint_64<id>::bt;
} // namespace mitsuha
#endif // AJAY_DYNAMIC_MODINT_64