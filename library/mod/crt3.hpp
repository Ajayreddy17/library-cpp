#ifndef AJAY_CRT3
#define AJAY_CRT3

namespace mitsuha{
constexpr unsigned int mod_pow_constexpr(unsigned long long a, unsigned long long n, unsigned long long mod) {
    a %= mod;
    unsigned long long res = 1;
    for (int _ = 0; _ < 32; ++_) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod, n /= 2;
    }
    return res;
}

template <typename T, unsigned int p0, unsigned int p1, unsigned int p2>
T CRT3(unsigned long long a0, unsigned long long a1, unsigned long long a2) {
    static_assert(p0 < p1 && p1 < p2);
    static constexpr unsigned long long x0_1 = mod_pow_constexpr(p0, p1 - 2, p1);
    static constexpr unsigned long long x01_2 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p2, p2 - 2, p2);
    unsigned long long c = (a1 - a0 + p1) * x0_1 % p1;
    unsigned long long a = a0 + c * p0;
    c = (a2 - a % p2 + p2) * x01_2 % p2;
    return T(a) + T(c) * T(p0) * T(p1);
}
} // namespace mitsuha
#endif // AJAY_CRT3