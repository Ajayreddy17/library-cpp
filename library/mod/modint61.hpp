#ifndef AJAY_MODINT61
#define AJAY_MODINT61

namespace mitsuha{
struct modint61 {
    static constexpr unsigned long long mod = (1ULL << 61) - 1;
    unsigned long long val;
    constexpr modint61() : val(0ULL) {}
    constexpr modint61(unsigned int x) : val(x) {}
    constexpr modint61(unsigned long long x) : val(x % mod) {}
    constexpr modint61(int x) : val((x < 0) ? (x + static_cast<long long>(mod)) : x) {}
    constexpr modint61(long long x) : val(((x %= static_cast<long long>(mod)) < 0) ? (x + static_cast<long long>(mod)): x) {}
    static constexpr unsigned long long get_mod() { return mod; }
    modint61 &operator+=(const modint61 &a) {
        val = ((val += a.val) >= mod) ? (val - mod) : val;
        return *this;
    }
    modint61 &operator-=(const modint61 &a) {
        val = ((val -= a.val) >= mod) ? (val + mod) : val;
        return *this;
    }
    modint61 &operator*=(const modint61 &a) {
        const unsigned __int128 y = static_cast<unsigned __int128>(val) * a.val;
        val = (y >> 61) + (y & mod);
        val = (val >= mod) ? (val - mod) : val;
        return *this;
    }
    modint61 operator-() const { return modint61(val ? mod - val : 0ULL); }
    modint61 &operator/=(const modint61 &a) { return (*this *= a.inverse()); }
    modint61 operator+(const modint61 &p) const { return modint61(*this) += p; }
    modint61 operator-(const modint61 &p) const { return modint61(*this) -= p; }
    modint61 operator*(const modint61 &p) const { return modint61(*this) *= p; }
    modint61 operator/(const modint61 &p) const { return modint61(*this) /= p; }
    bool operator==(const modint61 &p) const { return val == p.val; }
    bool operator!=(const modint61 &p) const { return val != p.val; }
    modint61 inverse() const {
        long long a = val, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        return modint61(u);
    }
    modint61 pow(long long n) const {
        assert(n >= 0);
        modint61 ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul, n >>= 1;
        }
        return ret;
    }
};

void read(modint61 &number){
    io::rd(number.val);
}
void write(const modint61 &number){
    io::wt(number.val);
}
ostream &operator<<(ostream &out, const modint61 &number){ return out << number.val; }
} // namespace mitsuha
#endif // AJAY_MODINT61