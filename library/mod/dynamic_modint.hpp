#ifndef AJAY_DYNAMIC_MODINT
#define AJAY_DYNAMIC_MODINT

#include "library/mod/modint_common.hpp"
#include "library/mod/primitive_root.hpp"
#include "library/mod/barrett.hpp"

namespace mitsuha {
template <int id>
struct Dynamic_Modint {
    static constexpr bool is_modint = true;
    using mint = Dynamic_Modint;
    unsigned int val;
    static Barrett bt;
    static unsigned int umod() { return bt.umod(); }

    static int get_mod() { return (int)(bt.umod()); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = Barrett(m);
    }

    static Dynamic_Modint raw(unsigned int v) {
        Dynamic_Modint x;
        x.val = v;
        return x;
    }
    Dynamic_Modint() : val(0) {}
    Dynamic_Modint(unsigned int x) : val(bt.modulo(x)) {}
    Dynamic_Modint(unsigned long long x) : val(bt.modulo(x)) {}
    Dynamic_Modint(int x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}
    Dynamic_Modint(long long x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}

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
        mint x = *this, r = 1;
        while (n) {
            if (n & 1)
                r *= x;
            x *= x, n >>= 1;
        }
        return r;
    }
    mint inverse() const {
        int x = val, mod = get_mod();
        int a = x, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        if (u < 0) u += mod;
        return u;
    }

    friend mint operator+(const mint& lhs, const mint& rhs) { return mint(lhs) += rhs; }
    friend mint operator-(const mint& lhs, const mint& rhs) { return mint(lhs) -= rhs; }
    friend mint operator*(const mint& lhs, const mint& rhs) { return mint(lhs) *= rhs; }
    friend mint operator/(const mint& lhs, const mint& rhs) { return mint(lhs) /= rhs; }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs.val != rhs.val;
    }
    static pair<int, int>& get_ntt() {
        static pair<int, int> p = {-1, -1};
        return p;
    }
    static void set_ntt_info() {
        int mod = get_mod();
        int k = ((mod - 1) == 0 ? -1 : __builtin_ctz(mod - 1));
        int r = primitive_root(mod);
        r = mod_pow(r, (mod - 1) >> k, mod);
        get_ntt() = {k, r};
    }
    static pair<int, int> ntt_info() { return get_ntt(); }
    static bool can_ntt() { return ntt_info().first != -1; }
};

template <int id>
void rd(Dynamic_Modint<id>& x) {
    io::rd(x.val);
    x.val %= Dynamic_Modint<id>::umod();
}
template <int id>
void wt(Dynamic_Modint<id> x) {
    io::wt(x.val);
}

template <int id>
ostream& operator<<(ostream& out, const Dynamic_Modint<id>& number) {
    return out << number.val;
}

using dmint = Dynamic_Modint<-1>;
template <int id>
Barrett Dynamic_Modint<id>::bt;
} // namespace mitsuha
#endif // AJAY_DYNAMIC_MODINT