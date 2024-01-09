#ifndef AJAY_MODINT
#define AJAY_MODINT

#include "library/mod/modint_common.hpp"

namespace mitsuha{
template <int mod>
struct modint {
    static constexpr unsigned int umod = (unsigned int)(mod);
    static_assert(umod < 1U << 31);
    unsigned int val;

    static modint raw(unsigned int v) {
        modint x;
        x.val = v;
        return x;
    }
    constexpr modint() : val(0) {}
    constexpr modint(unsigned int x) : val(x % umod) {}
    constexpr modint(unsigned long long x) : val(x % umod) {}
    constexpr modint(unsigned __int128 x) : val(x % umod) {}
    constexpr modint(int x) : val((x %= mod) < 0 ? x + mod : x){};
    constexpr modint(long long x) : val((x %= mod) < 0 ? x + mod : x){};
    constexpr modint(__int128 x) : val((x %= mod) < 0 ? x + mod : x){};
    bool operator<(const modint &other) const { return val < other.val; }
    modint &operator+=(const modint &p) {
        if ((val += p.val) >= umod) val -= umod;
        return *this;
    }
    modint &operator-=(const modint &p) {
        if ((val += umod - p.val) >= umod) val -= umod;
        return *this;
    }
    modint &operator*=(const modint &p) {
        val = (unsigned long long)(val) * p.val % umod;
        return *this;
    }
    modint &operator/=(const modint &p) {
        *this *= p.inverse();
        return *this;
    }
    modint operator-() const { return modint::raw(val ? mod - val : 0U); }
    modint operator+(const modint &p) const { return modint(*this) += p; }
    modint operator-(const modint &p) const { return modint(*this) -= p; }
    modint operator*(const modint &p) const { return modint(*this) *= p; }
    modint operator/(const modint &p) const { return modint(*this) /= p; }
    bool operator==(const modint &p) const { return val == p.val; }
    bool operator!=(const modint &p) const { return val != p.val; }
    modint inverse() const {
        int a = val, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        return modint(u);
    }
    modint pow(long long n) const {
        assert(n >= 0);
        modint ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
    static constexpr int get_mod() { return mod; }
   // (n, r), r is the 2^nth root of 1
    static constexpr pair<int, int> ntt_info() {
        if (mod == 120586241) return {20, 74066978};
        if (mod == 167772161) return {25, 17};
        if (mod == 469762049) return {26, 30};
        if (mod == 754974721) return {24, 362};
        if (mod == 880803841) return {23, 211};
        if (mod == 943718401) return {22, 663003469};
        if (mod == 998244353) return {23, 31};
        if (mod == 1045430273) return {20, 363};
        if (mod == 1051721729) return {20, 330};
        if (mod == 1053818881) return {20, 2789};
        return {-1, -1};
    }
    static constexpr bool can_ntt() { return ntt_info().first != -1; }
};

template<int _mod>  
void read(modint<_mod> &number){
    io::read(number.val);
    number.val %= _mod;
}
template<int _mod>
void write(const modint<_mod> &number){
    io::write(number.val);
}
template<int _mod>
ostream &operator<<(ostream &out, const modint<_mod> &number){ return out << number.val; }

using modint107 = modint<1000000007>;
using modint998 = modint<998244353>;
} // namespace mitsuha
#endif // AJAY_MODINT

