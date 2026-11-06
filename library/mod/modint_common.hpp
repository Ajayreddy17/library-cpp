#ifndef AJAY_MODINT_COMMON
#define AJAY_MODINT_COMMON

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/modint_common.hpp
struct has_mod_impl {
    template <class T>
    static auto check(T &&x) -> decltype(x.get_mod(), std::true_type{});
    template <class T>
    static auto check(...) -> std::false_type;
};

template <class T>
class has_mod : public decltype(has_mod_impl::check<T>(std::declval<T>())) {};

// the tables below are rebuilt when mint::get_mod() changes (Dynamic_Modint)
template <typename mint>
mint inv(int n) {
    static vector<mint> dat = {0, 1};
    static long long mod = 0;
    if (mod != mint::get_mod()) {
        mod = mint::get_mod();
        dat = {0, 1};
    }
    assert(0 <= n);
    if (n >= mod) n %= mod;
    while (len(dat) <= n) {
        long long k = len(dat);
        long long q = (mod + k - 1) / k;
        dat.emplace_back(dat[k * q - mod] * mint::raw(q));
    }
    return dat[n];
}

template <>
inline double inv<double>(int n) {
    assert(n != 0);
    return 1.0 / n;
}

template <typename mint>
mint fact(int n) {
    static vector<mint> dat = {1, 1};
    static long long mod = 0;
    if (mod != mint::get_mod()) {
        mod = mint::get_mod();
        dat = {1, 1};
    }
    assert(0 <= n && n < mod);
    if (len(dat) <= n) {
        int now = len(dat);
        int m = min<long long>(mod, 1LL << (__lg(n) + 1));
        dat.resize(m);
        For(i, now, m) dat[i] = dat[i - 1] * mint::raw(i);
    }
    return dat[n];
}

template <typename mint>
mint fact_inv(int n) {
    static vector<mint> dat = {1, 1};
    static long long mod = 0;
    if (n < 0) return mint(0);
    if (mod != mint::get_mod()) {
        mod = mint::get_mod();
        dat = {1, 1};
    }
    assert(n < mod);
    if (len(dat) <= n) {
        int now = len(dat);
        int m = min<long long>(mod, 1LL << (__lg(n) + 1));
        dat.resize(m);
        dat[m - 1] = fact<mint>(m - 1).inverse();
        Frr(i, now, m - 1) dat[i] = dat[i + 1] * mint::raw(i + 1);
    }
    return dat[n];
}

template <class mint, class... Ts>
mint fact_invs(Ts... xs) {
    return (mint(1) * ... * fact_inv<mint>(xs));
}

template <typename mint, class Head, class... Tail>
mint multinomial(Head &&head, Tail &&... tail) {
    return fact<mint>(head) * fact_invs<mint>(std::forward<Tail>(tail)...);
}

template <typename mint>
mint C_dense(int n, int k) {
    assert(n >= 0);
    if (k < 0 || n < k) return 0;
    static vector<vector<mint>> C;
    static int H = 0, W = 0;
    static long long mod = 0;
    if (mod != mint::get_mod()) {
        mod = mint::get_mod();
        C.clear();
        H = W = 0;
    }
    auto calc = [&](int i, int j) -> mint {
        if (i == 0) return (j == 0 ? mint(1) : mint(0));
        return C[i - 1][j] + (j ? C[i - 1][j - 1] : 0);
    };
    if (W <= k) {
        for(int i = 0; i < H; ++i) {
            C[i].resize(k + 1);
            for(int j = W; j < k + 1; ++j) { C[i][j] = calc(i, j); }
        }
        W = k + 1;
    }
    if (H <= n) {
        C.resize(n + 1);
        for(int i = H; i < n + 1; ++i) {
            C[i].resize(W);
            for(int j = 0; j < W; ++j) { C[i][j] = calc(i, j); }
        }
        H = n + 1;
    }
    return C[n][k];
}

template <typename mint, bool large = false, bool dense = false>
mint C(long long n, long long k) {
    assert(n >= 0);
    if (k < 0 || n < k) return 0;
    if constexpr (dense) return C_dense<mint>(n, k);
    if constexpr (!large) return multinomial<mint>(n, k, n - k);
    k = min(k, n - k);
    mint x(1);
    for(int i = 0; i < k; ++i) x *= mint(n - i);
    return x * fact_inv<mint>(k);
}

template <typename mint, bool large = false>
mint C_inv(long long n, long long k) {
    assert(n >= 0);
    assert(0 <= k && k <= n);
    if (not large) return fact_inv<mint>(n) * fact<mint>(k) * fact<mint>(n - k);
    return mint(1) / C<mint, true>(n, k);
}

// [x^d](1-x)^{-n}
// ways to put d identical items into n distinguishable bins
template <typename mint, bool large = false, bool dense = false>
mint C_negative(long long n, long long d) {
    assert(n >= 0);
    if (d < 0) return mint(0);
    if (n == 0) { return (d == 0 ? mint(1) : mint(0)); }
    return C<mint, large, dense>(n + d - 1, d);
}

template <typename mint, bool large = false>
mint P(long long n, long long k) {
    assert(n >= 0);
    if (k < 0 || n < k) return 0;
    if constexpr (!large) return multinomial<mint>(n, n - k);
    mint res = mint{1};
    for(auto i = n; i > n - k; -- i) res *= mint(i);
    return res;
}
} // namespace mitsuha
#endif // AJAY_MODINT_COMMON
