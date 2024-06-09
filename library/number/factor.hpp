#ifndef AJAY_FACTOR
#define AJAY_FACTOR

#include "library/number/primetest.hpp"
#include "library/random/base.hpp"

namespace mitsuha{
template <typename mint>
long long rho(long long n, long long c) {
    assert(n > 1);
    const mint cc(c);
    auto f = [&](mint x) { return x * x + cc; };
    mint x = 1, y = 2, z = 1, q = 1;
    long long g = 1;
    const long long m = 1LL << (__lg(n) / 5);
    for (long long r = 1; g == 1; r <<= 1) {
        x = y;
        For(r) y = f(y);
        for (long long k = 0; k < r && g == 1; k += m) {
            z = y;
            For(min(m, r - k)) y = f(y), q *= x - y;
            g = gcd(q.val(), n);
        }
    }
    if (g == n) do {
            z = f(z);
            g = gcd((x - z).val(), n);
        } while (g == 1);
    return g;
}

long long find_prime_factor(long long n) {
    assert(n > 1);
    if (primetest(n)) return n;
    For(100) {
        long long m = 0;
        if (n < (1 << 30)) {
            using mint = Mongomery_modint_32<20231025>;
            mint::set_mod(n);
            m = rho<mint>(n, RNG(0, n));
        } else {
            using mint = Mongomery_modint_64<20231025>;
            mint::set_mod(n);
            m = rho<mint>(n, RNG(0, n));
        }
        if (primetest(m)) return m;
        n = m;
    }
    assert(0);
    return -1;
}

// returns sorted
vector<pair<long long, int>> factor(long long n) {
    assert(n >= 1);
    vector<pair<long long, int>> pf;
    For(p, 2, 100) {
        if (p * p > n) break;
        if (n % p == 0) {
            long long e = 0;
            do { n /= p, e += 1; } while (n % p == 0);
            pf.emplace_back(p, e);
        }
    }
    while (n > 1) {
        long long p = find_prime_factor(n);
        long long e = 0;
        do { n /= p, e += 1; } while (n % p == 0);
        pf.emplace_back(p, e);
    }
    sort(pf.begin(), pf.end());
    return pf;
}

// doesn't sort
vector<pair<long long, int>> factor_by_lpf(long long n, vector<int>& lpf) {
    vector<pair<long long, int>> res;
    while (n > 1) {
        int p = lpf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        res.emplace_back(p, e);
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_FACTOR