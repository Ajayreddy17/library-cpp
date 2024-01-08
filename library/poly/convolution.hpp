#ifndef AJAY_CONVOLUTION
#define AJAY_CONVOLUTION

#include "library/mod/modint.hpp"
#include "library/mod/mod_inv.hpp"
#include "library/mod/crt3.hpp"
#include "library/poly/convolution_naive.hpp"
#include "library/poly/convolution_karatsuba.hpp"
#include "library/poly/ntt.hpp"
#include "library/poly/fft.hpp"

namespace mitsuha{
template <class mint>
vector<mint> convolution_ntt(vector<mint> a, vector<mint> b) {
    if (a.empty() || b.empty()) return {};
    int n = int(a.size()), m = int(b.size());
    int sz = 1;
    while (sz < n + m - 1) sz *= 2;

    // Speedup when sz = 2^k. Because it's a divide-and-conquer type of thing, you're going to lose a lot of money.
    if ((n + m - 3) <= sz / 2) {
        auto a_last = a.back(), b_last = b.back();
        a.pop_back(), b.pop_back();
        auto c = convolution(a, b);
        c.resize(n + m - 1);
        c[n + m - 2] = a_last * b_last;
        For(i, len(a)) c[i + len(b)] += a[i] * b_last;
        For(i, len(b)) c[i + len(a)] += b[i] * a_last;
        return c;
    }

    a.resize(sz), b.resize(sz);
    bool same = a == b;
    ntt(a, 0);
    if (same) {
        b = a;
    } else {
        ntt(b, 0);
    }
    For(i, sz) a[i] *= b[i];
    ntt(a, 1);
    a.resize(n + m - 1);
    return a;
}

template <typename mint>
vector<mint> convolution_garner(const vector<mint>& a, const vector<mint>& b) {
    int n = len(a), m = len(b);
    if (!n || !m) return {};
    static constexpr int p0 = 167772161;
    static constexpr int p1 = 469762049;
    static constexpr int p2 = 754974721;
    using mint0 = modint<p0>;
    using mint1 = modint<p1>;
    using mint2 = modint<p2>;
    vector<mint0> a0(n), b0(m);
    vector<mint1> a1(n), b1(m);
    vector<mint2> a2(n), b2(m);
    For(i, n) a0[i] = a[i].val, a1[i] = a[i].val, a2[i] = a[i].val;
    For(i, m) b0[i] = b[i].val, b1[i] = b[i].val, b2[i] = b[i].val;
    auto c0 = convolution_ntt<mint0>(a0, b0);
    auto c1 = convolution_ntt<mint1>(a1, b1);
    auto c2 = convolution_ntt<mint2>(a2, b2);
    vector<mint> c(len(c0));
    For(i, n + m - 1) {
        c[i] = CRT3<mint, p0, p1, p2>(c0[i].val, c1[i].val, c2[i].val);
    }
    return c;
}

template <typename R>
vector<double> convolution_fft(const vector<R>& a, const vector<R>& b) {
    using C = CFFT::C;
    int need = (int)a.size() + (int)b.size() - 1;
    int nbase = 1;
    while ((1 << nbase) < need) nbase++;
    CFFT::ensure_base(nbase);
    int sz = 1 << nbase;
    vector<C> fa(sz);
    for (int i = 0; i < sz; i++) {
        double x = (i < (int)a.size() ? a[i] : 0);
        double y = (i < (int)b.size() ? b[i] : 0);
        fa[i] = C(x, y);
    }
    CFFT::fft(fa, sz);
    C r(0, -0.25 / (sz >> 1)), s(0, 1), t(0.5, 0);
    for (int i = 0; i <= (sz >> 1); i++) {
        int j = (sz - i) & (sz - 1);
        C z = (fa[j] * fa[j] - (fa[i] * fa[i]).conj()) * r;
        fa[j] = (fa[i] * fa[i] - (fa[j] * fa[j]).conj()) * r;
        fa[i] = z;
    }
    for (int i = 0; i < (sz >> 1); i++) {
        C A0 = (fa[i] + fa[i + (sz >> 1)]) * t;
        C A1 = (fa[i] - fa[i + (sz >> 1)]) * t * CFFT::rts[(sz >> 1) + i];
        fa[i] = A0 + A1 * s;
    }
    CFFT::fft(fa, sz >> 1);
    vector<double> ret(need);
    for (int i = 0; i < need; i++) {
        ret[i] = (i & 1 ? fa[i >> 1].y : fa[i >> 1].x);
    }
    return ret;
}

vector<long long> convolution(const vector<long long>& a, const vector<long long>& b) {
    int n = len(a), m = len(b);
    if (!n || !m) return {};
    if (min(n, m) <= 2500) return convolution_naive(a, b);
    long long abs_sum_a = 0, abs_sum_b = 0;
    long long LIM = 1e15;
    For(i, n) abs_sum_a = min(LIM, abs_sum_a + abs(a[i]));
    For(i, m) abs_sum_b = min(LIM, abs_sum_b + abs(b[i]));
    if (__int128(abs_sum_a) * abs_sum_b < 1e15) {
        vector<double> c = convolution_fft<long long>(a, b);
        vector<long long> res(len(c));
        For(i, len(c)) res[i] = (long long)(std::floor(c[i] + .5));
        return res;
    }

    static constexpr unsigned long long MOD1 = 754974721; // 2^24
    static constexpr unsigned long long MOD2 = 167772161; // 2^25
    static constexpr unsigned long long MOD3 = 469762049; // 2^26
    static constexpr unsigned long long M2M3 = MOD2 * MOD3;
    static constexpr unsigned long long M1M3 = MOD1 * MOD3;
    static constexpr unsigned long long M1M2 = MOD1 * MOD2;
    static constexpr unsigned long long M1M2M3 = MOD1 * MOD2 * MOD3;

    static const unsigned long long i1 = mod_inv(MOD2 * MOD3, MOD1);
    static const unsigned long long i2 = mod_inv(MOD1 * MOD3, MOD2);
    static const unsigned long long i3 = mod_inv(MOD1 * MOD2, MOD3);

    using mint1 = modint<MOD1>;
    using mint2 = modint<MOD2>;
    using mint3 = modint<MOD3>;

    vector<mint1> a1(n), b1(m);
    vector<mint2> a2(n), b2(m);
    vector<mint3> a3(n), b3(m);
    For(i, n) a1[i] = a[i], a2[i] = a[i], a3[i] = a[i];
    For(i, m) b1[i] = b[i], b2[i] = b[i], b3[i] = b[i];

    auto c1 = convolution_ntt<mint1>(a1, b1);
    auto c2 = convolution_ntt<mint2>(a2, b2);
    auto c3 = convolution_ntt<mint3>(a3, b3);

    vector<long long> c(n + m - 1);
    For(i, n + m - 1) {
        unsigned long long x = 0;
        x += (c1[i].val * i1) % MOD1 * M2M3;
        x += (c2[i].val * i2) % MOD2 * M1M3;
        x += (c3[i].val * i3) % MOD3 * M1M2;
        long long diff = c1[i].val - ((long long)(x) % (long long)(MOD1));
        if (diff < 0) diff += MOD1;
        static constexpr unsigned long long offset[5]
                = {0, 0, M1M2M3, 2 * M1M2M3, 3 * M1M2M3};
        x -= offset[diff % 5];
        c[i] = x;
    }
    return c;
}

template <typename mint>
vector<mint> convolution(const vector<mint>& a, const vector<mint>& b) {
    int n = len(a), m = len(b);
    if (!n || !m) return {};
    if (mint::can_ntt()) {
        if (min(n, m) <= 50) return convolution_karatsuba<mint>(a, b);
        return convolution_ntt(a, b);
    }
    if (min(n, m) <= 200) return convolution_karatsuba<mint>(a, b);
    return convolution_garner(a, b);
}
} // namespace mitsuha
#endif // AJAY_CONVOLUTION