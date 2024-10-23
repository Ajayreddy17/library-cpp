#ifndef AJAY_CRT3
#define AJAY_CRT3

namespace mitsuha{
constexpr unsigned int mod_pow_constexpr(unsigned long long a, unsigned long long n, unsigned int mod) {
    a %= mod;
    unsigned long long res = 1;
    For(32) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod, n /= 2;
    }
    return res;
}

template <typename T, unsigned int p0, unsigned int p1>
T CRT2(unsigned long long a0, unsigned long long a1) {
    static_assert(p0 < p1);
    static constexpr unsigned long long x0_1 = mod_pow_constexpr(p0, p1 - 2, p1);
    unsigned long long c = (a1 - a0 + p1) * x0_1 % p1;
    return a0 + c * p0;
}

template <typename T, unsigned int p0, unsigned int p1, unsigned int p2>
T CRT3(unsigned long long a0, unsigned long long a1, unsigned long long a2) {
    static_assert(p0 < p1 && p1 < p2);
    static constexpr unsigned long long x1 = mod_pow_constexpr(p0, p1 - 2, p1);
    static constexpr unsigned long long x2 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p2, p2 - 2, p2);
    static constexpr unsigned long long p01 = (unsigned long long)(p0) * p1;
    unsigned long long c = (a1 - a0 + p1) * x1 % p1;
    unsigned long long ans_1 = a0 + c * p0;
    c = (a2 - ans_1 % p2 + p2) * x2 % p2;
    return T(ans_1) + T(c) * T(p01);
}

template <typename T, unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3>
T CRT4(unsigned long long a0, unsigned long long a1, unsigned long long a2, unsigned long long a3) {
    static_assert(p0 < p1 && p1 < p2 && p2 < p3);
    static constexpr unsigned long long x1 = mod_pow_constexpr(p0, p1 - 2, p1);
    static constexpr unsigned long long x2 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p2, p2 - 2, p2);
    static constexpr unsigned long long x3 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p3 * p2 % p3, p3 - 2, p3);
    static constexpr unsigned long long p01 = (unsigned long long)(p0) * p1;
    unsigned long long c = (a1 - a0 + p1) * x1 % p1;
    unsigned long long ans_1 = a0 + c * p0;
    c = (a2 - ans_1 % p2 + p2) * x2 % p2;
    unsigned __int128 ans_2 = ans_1 + c * static_cast<unsigned __int128>(p01);
    c = (a3 - ans_2 % p3 + p3) * x3 % p3;
    return T(ans_2) + T(c) * T(p01) * T(p2);
}

template <typename T, unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4>
T CRT5(unsigned long long a0, unsigned long long a1, unsigned long long a2, unsigned long long a3, unsigned long long a4) {
    static_assert(p0 < p1 && p1 < p2 && p2 < p3 && p3 < p4);
    static constexpr unsigned long long x1 = mod_pow_constexpr(p0, p1 - 2, p1);
    static constexpr unsigned long long x2 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p2, p2 - 2, p2);
    static constexpr unsigned long long x3 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p3 * p2 % p3, p3 - 2, p3);
    static constexpr unsigned long long x4 = mod_pow_constexpr((unsigned long long)(p0) * p1 % p4 * p2 % p4 * p3 % p4, p4 - 2, p4);
    static constexpr unsigned long long p01 = (unsigned long long)(p0) * p1;
    static constexpr unsigned long long p23 = (unsigned long long)(p2) * p3;
    unsigned long long c = (a1 - a0 + p1) * x1 % p1;
    unsigned long long ans_1 = a0 + c * p0;
    c = (a2 - ans_1 % p2 + p2) * x2 % p2;
    unsigned __int128 ans_2 = ans_1 + c * static_cast<unsigned __int128>(p01);
    c = static_cast<unsigned long long>(a3 - ans_2 % p3 + p3) * x3 % p3;
    unsigned __int128 ans_3 = ans_2 + static_cast<unsigned __int128>(c * p2) * p01;
    c = static_cast<unsigned long long>(a4 - ans_3 % p4 + p4) * x4 % p4;
    return T(ans_3) + T(c) * T(p01) * T(p23);
}
} // namespace mitsuha
#endif // AJAY_CRT3