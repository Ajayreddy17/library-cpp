#ifndef AJAY_MODINT_64BIT
#define AJAY_MODINT_64BIT

namespace mitsuha{
// https://qoj.ac/problem/969
template <long long mod>
struct modint_64bit {
    using T = modint_64bit;
    static constexpr unsigned long long umod = (unsigned long long)(mod);
    static_assert(umod < 1ULL << 63);
    unsigned long long val;
    constexpr modint_64bit() : val(0) {}
    constexpr modint_64bit(unsigned int x) : val(x % umod) {}
    constexpr modint_64bit(unsigned long long x) : val(x % umod) {}
    constexpr modint_64bit(unsigned __int128 x) : val(x % umod) {}
    constexpr modint_64bit(int x) : val((x %= mod) < 0 ? x + mod : x) {};
    constexpr modint_64bit(long long x) : val((x %= mod) < 0 ? x + mod : x) {};
    constexpr modint_64bit(__int128 x) : val((x %= mod) < 0 ? x + mod : x) {};
    static T raw(unsigned long long v) {
        T x;
        x.val = v;
        return x;
    }
    bool operator<(const T& other) const { return val < other.val; }
    T& operator+=(const T& p) {
        if ((val += p.val) >= umod)
            val -= umod;
        return *this;
    }
    T& operator-=(const T& p) {
        if ((val += umod - p.val) >= umod)
            val -= umod;
        return *this;
    }
    T& operator*=(const T& p) {
        val = (unsigned long long)(val) * p.val % umod;
        return *this;
    }
    T& operator/=(const T& p) {
        *this *= p.inverse();
        return *this;
    }
    T operator-() const { return raw(val ? mod - val : 0U); }
    T operator+(const T& p) const { return modint_64bit(*this) += p; }
    T operator-(const T& p) const { return modint_64bit(*this) -= p; }
    T operator*(const T& p) const { return modint_64bit(*this) *= p; }
    T operator/(const T& p) const { return modint_64bit(*this) /= p; }
    bool operator==(const T& p) const { return val == p.val; }
    bool operator!=(const T& p) const { return val != p.val; }
    T inverse() const {
        int a = val, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        return modint_64bit(u);
    }
    T pow(long long n) const {
        assert(n >= 0);
        T ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul, n >>= 1;
        }
        return ret;
    }
    static constexpr long long get_mod() { return mod; }
    // (n, r), where r is a 2^n-th root of 1
    static constexpr pair<long long, long long> ntt_info() { return {-1, -1}; }
    static constexpr bool can_ntt() { return ntt_info().first != -1; }
};

#ifdef FASTIO
template <long long mod>
void rd(modint_64bit<mod>& x) {
    io::rd(x.val);
    x.val %= mod;
}
template <long long mod>
void wt(modint_64bit<mod> x) {
    io::wt(x.val);
}
#endif
} // namespace mitsuha
#endif // AJAY_MODINT_64BIT