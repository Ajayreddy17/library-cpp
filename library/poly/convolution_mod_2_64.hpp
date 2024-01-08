#ifndef AJAY_CONV_MOD_2_64
#define AJAY_CONV_MOD_2_64

#include "library/poly/convolution.hpp"

namespace mitsuha{
vector<unsigned long long> convolution_mod_2_64(const vector<unsigned long long>& a, const vector<unsigned long long>& b) {
    int n = len(a), m = len(b);
    if (!n || !m) return {};
    if (min(n, m) <= 12500) return convolution_karatsuba(a, b);
    constexpr int P0 = 998244353;
    constexpr int P1 = 754974721;
    constexpr int P2 = 167772161;
    constexpr int P3 = 469762049;
    constexpr int P4 = 880803841;
    using M0 = modint<P0>;
    using M1 = modint<P1>;
    using M2 = modint<P2>;
    using M3 = modint<P3>;
    using M4 = modint<P4>;
    vector<M0> a0(n), b0(m);
    vector<M1> a1(n), b1(m);
    vector<M2> a2(n), b2(m);
    vector<M3> a3(n), b3(m);
    vector<M4> a4(n), b4(m);
    for(int i = 0; i < n; i++) a0[i] = a[i] % P0;
    for(int i = 0; i < m; i++) b0[i] = b[i] % P0;
    for(int i = 0; i < n; i++) a1[i] = a[i] % P1;
    for(int i = 0; i < m; i++) b1[i] = b[i] % P1;
    for(int i = 0; i < n; i++) a2[i] = a[i] % P2;
    for(int i = 0; i < m; i++) b2[i] = b[i] % P2;
    for(int i = 0; i < n; i++) a3[i] = a[i] % P3;
    for(int i = 0; i < m; i++) b3[i] = b[i] % P3;
    for(int i = 0; i < n; i++) a4[i] = a[i] % P4;
    for(int i = 0; i < m; i++) b4[i] = b[i] % P4;
    a0 = convolution_ntt<M0>(a0, b0);
    a1 = convolution_ntt<M1>(a1, b1);
    a2 = convolution_ntt<M2>(a2, b2);
    a3 = convolution_ntt<M3>(a3, b3);
    a4 = convolution_ntt<M4>(a4, b4);
    static const M1 inv10 = M1(1) / M1(P0);
    static const M2 inv21 = M2(1) / M2(P1), inv20 = inv21 / M2(P0);
    static const M3 inv32 = M3(1) / M3(P2), inv31 = inv32 / M3(P1),
            inv30 = inv31 / M3(P0);
    static const M4 inv43 = M4(1) / M4(P3), inv42 = inv43 / M4(P2),
            inv41 = inv42 / M4(P1), inv40 = inv41 / M4(P0);
    vector<unsigned long long> c(len(a0));
    for(int i = 0; i < len(c); i++) {
        long long x0 = a0[i].val;
        long long x1 = (M1(a1[i] - x0) * inv10).val;
        long long x2 = (M2(a2[i] - x0) * inv20 - M2(x1) * inv21).val;
        long long x3 = (M3(a3[i] - x0) * inv30 - M3(x1) * inv31 - M3(x2) * inv32).val;
        long long x4 = (M4(a4[i] - x0) * inv40 - M4(x1) * inv41 - M4(x2) * inv42 - M4(x3) * inv43).val;
        c[i] = x0 + P0 * (x1 + P1 * (x2 + P2 * (x3 + P3 * (unsigned long long)(x4))));
    }
    return c;
}
} // namespace mitsuha
#endif // AJAY_CONV_MOD_2_64