#ifndef AJAY_CONVOLUTION
#define AJAY_CONVOLUTION

#include "library/mod/modint.hpp"
#include "library/mod/mod_inv.hpp"
#include "library/mod/crt3.hpp"
#include "library/poly/convolution_naive.hpp"
#include "library/poly/convolution_karatsuba.hpp"
#include "library/poly/ntt.hpp"

namespace mitsuha {
template <class mint>
vector<mint> convolution_ntt(vector<mint> a, vector<mint> b) {
    if (a.empty() || b.empty()) return {};
    int n = int(a.size()), m = int(b.size());
    int sz = 1;
    while (sz < n + m - 1) sz *= 2;

    // Speeding up when sz = 2^k. 
    // The divide-and-conquer approach is causing a lot of inefficiencies.
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
    For(i, n + m - 1) { c[i] = CRT3<mint, p0, p1, p2>(c0[i].val, c1[i].val, c2[i].val); }
    return c;
}

vector<long long> convolution(vector<long long> a, vector<long long> b) {
    int n = len(a), m = len(b);
    if (!n || !m) return {};
    if (min(n, m) <= 2500) return convolution_naive(a, b);

    long long mi_a = *min_element(a.begin(), a.end()), mi_b = *min_element(b.begin(), b.end());
    for (auto& x : a) x -= mi_a;
    for (auto& x : b) x -= mi_b;
    assert((*max_element(a.begin(), a.end())) * (*max_element(b.begin(), b.end())) <= 1e18);

    vector<long long> Ac(a.size() + 1);
    vector<long long> Bc(a.size() + 1);
    For(x, a.size()) Ac[x + 1] = Ac[x] + a[x];
    For(x, b.size()) Bc[x + 1] = Bc[x] + b[x];
    vector<long long> res(n + m - 1);
    for (int k = 0; k < n + m - 1; ++k) {
        int s = max(0, k - m + 1);
        int t = min(n, k + 1);
        res[k] += (t - s) * mi_a * mi_b;
        res[k] += mi_a * (Bc[k - s + 1] - Bc[k - t + 1]);
        res[k] += mi_b * (Ac[t] - Ac[s]);
    }

    static constexpr unsigned int MOD1 = 1004535809;
    static constexpr unsigned int MOD2 = 1012924417;
    using mint1 = modint<MOD1>;
    using mint2 = modint<MOD2>;

    vector<mint1> a1(n), b1(m);
    vector<mint2> a2(n), b2(m);
    For(i, n) a1[i] = a[i], a2[i] = a[i];
    For(i, m) b1[i] = b[i], b2[i] = b[i];

    auto c1 = convolution_ntt<mint1>(a1, b1);
    auto c2 = convolution_ntt<mint2>(a2, b2);

    For(i, n + m - 1) { res[i] += CRT2<unsigned long long, MOD1, MOD2>(c1[i].val, c2[i].val); }
    return res;
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