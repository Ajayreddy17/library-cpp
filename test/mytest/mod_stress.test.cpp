#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/mod/modint.hpp"
#include "library/mod/modint_64bit.hpp"
#include "library/mod/dynamic_modint.hpp"
#include "library/mod/mod_pow.hpp"
#include "library/mod/mod_sqrt.hpp"
#include "library/mod/primitive_root.hpp"
#include "library/mod/mod_pow_u64.hpp"
#include "library/mod/mod_inv_u64.hpp"
#include "library/mod/mod_log_u64.hpp"
#include "library/mod/binomial_u64.hpp"
#include "library/mod/O1_inverse.hpp"
#include "library/mod/range_freq_mod.hpp"
#include "library/mod/mod_sum_of_linear.hpp"
#include "library/mod/range_freq_of_linear.hpp"
#include "library/mod/min_of_linear.hpp"
#include "library/mod/max_of_linear_segments.hpp"
#include "library/mod/first_mod_range_of_linear.hpp"
#include "library/mod/floor_sum_of_linear_polynomial.hpp"
#include "library/mod/floor_sum_of_linear_polynomial_pq.hpp"
#include "library/mod/q_analogue.hpp"
#include "library/mod/binomial.hpp"
#include "library/mod/prefix_sum_of_binom.hpp"
#include "library/mod/multiplicative_convolution_mod_p.hpp"
#include "library/mod/multiplicative_convolution_mod_2n.hpp"
#include "library/mod/mod_kth_root.hpp"
#include "library/mod/modular_subset_sum.hpp"
#include "library/mod/modfast.hpp"

using mint = modint998;

long long brute_pow(long long a, long long n, long long m) {
    __int128 r = 1 % m, x = rem(a, m);
    while (n) {
        if (n & 1) r = r * x % m;
        x = x * x % m, n >>= 1;
    }
    return r;
}

void test_modint() {
    // modint_64bit with a modulus near 2^60: products need 128 bits
    using M = modint_64bit<1000000000000000003LL>;
    const long long P = 1000000000000000003LL;
    For(1000) {
        long long a = RNG(1, P), b = RNG(0, P);
        assert((M(a) * M(b)).val == (unsigned long long)((__int128)a * b % P));
        assert((M(a) * M(a).inverse()).val == 1);
        long long n = RNG(0, 100);
        assert(M(a).pow(-n) * M(a).pow(n) == M(1));
    }
    For(100) {
        int a = RNG(1, 998244353);
        long long n = RNG(0, 1000);
        assert(mint(a).pow(-n) == mint(a).pow(n).inverse());
    }
    // modint_common follows Dynamic_Modint mod changes
    for (int m: {7, 13, 1000003, 11}) {
        dmint::set_mod(m);
        long long f = 1;
        For(i, 1, min(m, 50)) {
            f = f * i % m;
            assert(fact<dmint>(i).val == f);
            assert((fact_inv<dmint>(i) * fact<dmint>(i)).val == 1);
            assert((inv<dmint>(i) * dmint(i)).val == 1);
        }
        For(n, min(m, 20)) For(k, n + 1) {
            assert((C<dmint>(n, k) == C<dmint, false, true>(n, k)));
        }
    }
    assert(mod_pow(5, 0, 1) == 0 && mod_pow_64(5, 0, 1) == 0);
    For(100) {
        long long a = RNG(-1000000, 1000000), n = RNG(0, 1000000), m = RNG(1, 1000000000);
        assert(mod_pow(a, n, m) == brute_pow(a, n, m));
        long long m2 = RNG(1, 1LL << 62);
        assert((long long)mod_pow_64(a, n, m2) == brute_pow(a, n, m2));
    }
}

void test_roots() {
    // mod_sqrt_64 needs D %= p: big primes
    for (long long p: {1000000000000000003LL, 998244353LL, 4611686018427387847LL}) {
        For(200) {
            long long x = RNG(0, p);
            long long a = (__int128)x * x % p;
            long long r = mod_sqrt_64(a, p);
            assert(r != -1 && (__int128)r * r % p == a);
        }
    }
    // smallest primitive root
    for (int p: {2, 3, 5, 7, 11, 13, 23, 41, 71, 191, 409, 998244353}) {
        int g = primitive_root(p);
        int brute = -1;
        For(x, 1, p) {
            bool ok = true;
            long long y = 1;
            For(e, 1, p - 1) {
                y = y * x % p;
                if (y == 1) { ok = false; break; }
            }
            if (ok) { brute = x; break; }
            if (p > 1000) break;
        }
        if (p < 1000) assert(g == brute);
        if (p == 998244353) assert(g == 3);
    }
    // primitive root mod p^e: order is phi(p^e)
    for (auto [p, e]: vector<pair<int, int>>{{3, 4}, {5, 3}, {7, 2}, {11, 2}, {3, 7}}) {
        long long q = 1;
        For(e) q *= p;
        long long phi = q / p * (p - 1);
        long long g = primitive_root_prime_power_64(p, e);
        long long y = 1;
        For(i, 1, phi + 1) {
            y = y * g % q;
            assert((y == 1) == (i == phi));
        }
    }
    // kth root mod prime
    for (int p: {2, 3, 5, 7, 13, 17, 97, 101, 769}) {
        For(k, 0, 40) For(a, p) {
            int r = mod_kth_root(k, a, p);
            bool exists = false;
            For(x, p) if (brute_pow(x, k, p) == a) exists = true;
            if (k == 0) exists = (a == 1);
            if (!exists) assert(r == -1);
            if (exists) assert(r != -1 && brute_pow(r, k, p) == a);
        }
    }
    for (long long p: {998244353LL, 1000000007LL, 1000000000000000003LL, 4611686018427387847LL}) {
        For(200) {
            long long k = RNG(0, 1LL << 60), x = RNG(0, p);
            long long a = brute_pow(x, k, p);
            long long r = (p < (1LL << 31) ? mod_kth_root(k, a, p) : mod_kth_root_64(k, a, p));
            assert(r != -1 && brute_pow(r, k, p) == a);
        }
    }
}

void test_u64() {
    For(1000) {
        unsigned long long a = RNG_64(), n = RNG(0, 1000);
        unsigned long long x = 1;
        For(n) x *= a;
        assert(mod_pow_u64(a, n) == x);
        a |= 1;
        assert(a * mod_inv_u64(a) == 1);
    }
    For(1000) {
        unsigned long long a = RNG_64(), n = RNG(0, 1LL << 62);
        if (RNG(0, 4) == 0) a &= ~1ULL, n = RNG(0, 70);
        unsigned long long b = mod_pow_u64(a, n);
        long long m = mod_log_u64(a, b);
        assert(m != -1 && (unsigned long long)m <= n && mod_pow_u64(a, m) == b);
        if (a % 2 == 0) {
            // smallest exponent by brute force
            long long s = -1;
            unsigned long long x = 1;
            For(e, 70) {
                if (x == b) { s = e; break; }
                x *= a;
            }
            assert(m == s);
        }
    }
    // a^n never hits an even b when a is odd
    assert(mod_log_u64(3, 2) == -1);
    // binomial mod 2^64 = Pascal's triangle with wrapping arithmetic
    Binomial_u64 X(300);
    vector<unsigned long long> row = {1};
    For(n, 301) {
        For(k, -1, n + 2) assert(X.C(n, k) == (0 <= k && k <= n ? row[k] : 0));
        vector<unsigned long long> nrow(n + 2);
        For(k, n + 1) nrow[k] += row[k], nrow[k + 1] += row[k];
        row = nrow;
    }
}

void test_inverse() {
    For(100000) {
        int a = RNG(1, 998244353);
        assert(O1_inverse<mint>(a) * mint(a) == mint(1));
    }
    using M2 = modint<1000003>;
    For(a, 1, 1000003) assert(O1_inverse<M2>(a) * M2(a) == M2(1));
    // switching back to the first mod rebuilds the tables
    For(1000) {
        int a = RNG(1, 998244353);
        assert(O1_inverse<mint>(a) * mint(a) == mint(1));
    }
}

void test_linear_mod() {
    For(3000) {
        long long mod = RNG(1, 30), L = RNG(-50, 50), R = L + RNG(0, 50);
        long long a = RNG(-40, 40), b = RNG(-40, 40);
        long long lo = RNG(0, mod + 1), hi = RNG(lo, mod + 1);
        long long c1 = 0, c2 = 0, c3 = 0;
        __int128 s = 0;
        For(x, L, R) {
            long long r = rem(a * x + b, mod);
            s += r;
            if (lo <= r && r < hi) ++c2;
            if (lo <= rem(x, mod) && rem(x, mod) < hi) ++c1;
        }
        assert(range_freq_mod<long long>(L, R, lo, hi, mod) == c1);
        assert((range_freq_of_linear<__int128, long long>(L, R, a, b, mod, lo, hi) == c2));
        assert(mod_sum_of_linear(L, R, a, b, mod) == s);
        (void)c3;

        // min_of_linear: smallest argmin
        if (L < R) {
            auto [x, y] = min_of_linear(L, R, a, b, mod);
            long long bx = L, by = rem(a * L + b, mod);
            For(t, L, R) if (rem(a * t + b, mod) < by) bx = t, by = rem(a * t + b, mod);
            assert(x == bx && y == by);
        }
        // first x >= 0 with (ax+b) mod in [lo', hi')
        long long l2 = RNG(-40, 40), r2 = l2 + RNG(0, 40);
        long long first = -1;
        For(x, 0, 3 * mod + 3) {
            long long v = a * x + b;
            bool ok = false;
            For(t, l2, r2) if (rem(v - t, mod) == 0) ok = true;
            if (ok) { first = x; break; }
        }
        assert(first_mod_range_of_linear(l2, r2, a, b, mod) == first);
    }
    // prefix-min / prefix-max update points as unions of progressions
    For(mod, 1, 60) For(a, mod) For(b, mod) {
        For(t, 2) {
            auto [X, DX] = (t == 0 ? min_of_linear_segments(a, b, mod) : max_of_linear_segments(a, b, mod));
            vector<int> got = {X[0]};
            For(i, len(DX)) for (int x = X[i] + DX[i]; x <= X[i + 1]; x += DX[i]) got.emplace_back(x);
            assert(got.back() == X.back());
            vector<int> brute;
            int best = -1;
            For(x, X.back() + 1) {
                int v = (a * x + b) % mod;
                if (t == 1) v = mod - 1 - v;
                if (best == -1 || v < best) best = v, brute.emplace_back(x);
            }
            assert(got == brute);
            // nothing better afterwards
            For(x, X.back(), X.back() + mod + 1) {
                int v = (a * x + b) % mod;
                if (t == 1) v = mod - 1 - v;
                assert(v >= best);
            }
        }
    }
    // huge L: a * L must not overflow
    For(1000) {
        long long L = RNG(-(1LL << 60), 1LL << 60), n = RNG(1, 50), m = RNG(1, 1000000000), a = RNG(0, m), b = RNG(0, m);
        auto [x, y] = min_of_linear(L, L + n, a, b, m);
        long long bx = L, by = m;
        For(t, L, L + n) {
            long long v = ((__int128)a * t + b) % m;
            if (v < 0) v += m;
            if (v < by) bx = t, by = v;
        }
        assert(x == bx && y == by);
    }
    // large range
    For(1000) {
        long long n = RNG(1, 1000000000), m = RNG(1, 1000000000), a = RNG(0, m), b = RNG(0, m);
        auto [x, y] = min_of_linear(0, n, a, b, m);
        assert((__int128(a) * x + b) % m == y);
        // y is the min: no x' < n with value < y
        long long k = range_freq_of_linear<__int128, long long>(0, n, a, b, m, 0, y);
        assert(k == 0);
        assert((range_freq_of_linear<__int128, long long>(0, x, a, b, m, y, y + 1) == 0));
    }
}

void test_floor_sum_polynomial() {
    For(1000) {
        long long L = RNG(-30, 30), R = L + RNG(0, 30), mod = RNG(1, 20), a = RNG(-30, 30), b = RNG(-30, 30);
        auto A = floor_sum_of_linear_polynomial<mint, 2, 3, long long>(L, R, a, b, mod);
        mint p = RNG(1, 100), q = RNG(1, 100);
        auto B = floor_sum_of_linear_polynomial_pq<mint, 3, 2, long long>(p, q, L, R, a, b, mod);
        array<array<mint, 4>, 3> bA{};
        array<array<mint, 3>, 4> bB{};
        For(x, L, R) {
            long long y = fld(a * x + b, mod);
            For(i, 3) For(j, 4) bA[i][j] += mint(x).pow(i) * mint(y).pow(j);
            mint w = p.pow(x) * q.pow(y);
            For(i, 4) For(j, 3) bB[i][j] += w * mint(x).pow(i) * mint(y).pow(j);
        }
        assert(A == bA && B == bB);
    }
}

void test_binomials() {
    // q-binomials vs the q-Pascal recurrence
    using M = modint<29>;
    For(q0, 29) {
        M q = q0;
        int LIM = 28;
        q_Analogue<M> X(q, LIM);
        vector<vector<M>> T(LIM + 1, vector<M>(LIM + 1));
        For(n, LIM + 1) {
            T[n][0] = 1;
            For(k, 1, n + 1) T[n][k] = T[n - 1][k - 1] + q.pow(k) * T[n - 1][k];
        }
        For(n, LIM + 1) For(k, -1, n + 2) assert(X.binom(n, k) == (0 <= k && k <= n ? T[n][k] : M(0)));
    }
    // arbitrary mod binomials vs Pascal
    for (int m: {1, 2, 4, 8, 64, 27, 81, 6, 12, 100, 360, 997, 1024, 999, 1000000}) {
        Binomial X(m);
        vector<vector<int>> T(130, vector<int>(130));
        For(n, 130) {
            T[n][0] = 1 % m;
            For(k, 1, n + 1) T[n][k] = (T[n - 1][k - 1] + T[n - 1][k]) % m;
        }
        For(n, 130) For(k, -1, n + 2) assert(X.C(n, k) == (0 <= k && k <= n ? T[n][k] : 0));
    }
    // large n mod a prime vs Lucas
    Binomial X(1000003);
    For(1000) {
        long long n = RNG(0, 1LL << 60), k = RNG(0, n + 1);
        using M3 = modint<1000003>;
        M3 r = 1;
        long long a = n, b = k;
        while (a) {
            r *= C<M3>(a % 1000003, b % 1000003);
            a /= 1000003, b /= 1000003;
        }
        assert(X.C(n, k) == (int)r.val);
    }
    // prefix sums of binomials
    int N = 300;
    Prefix_Sum_of_Binom<mint> P(N);
    Prefix_Sum_of_Binom_Offline<mint> PO;
    vector<mint> expect;
    For(n, N + 1) {
        mint s = 0;
        For(m, n + 3) {
            assert(P.query(n, m) == s);
            if (RNG(0, 10) == 0) PO.add(n, m), expect.emplace_back(s);
            s += C<mint>(n, m);
        }
    }
    assert(PO.calc() == expect);
}

void test_convolution() {
    for (int p: {2, 3, 5, 7, 13, 97}) {
        vector<mint> A(p), B(p), C(p);
        For(i, p) A[i] = RNG(0, 1000), B[i] = RNG(0, 1000);
        For(i, p) For(j, p) C[i * j % p] += A[i] * B[j];
        assert(multiplicative_convolution_mod_p<mint>(p, A, B) == C);
    }
    For(N, 0, 9) {
        int n = 1 << N;
        vector<mint> A(n), B(n), C(n);
        For(i, n) A[i] = RNG(0, 1000), B[i] = RNG(0, 1000);
        For(i, n) For(j, n) C[i * j % n] += A[i] * B[j];
        assert(multiplicative_convolution_mod_2n(A, B) == C);
    }
}

void test_subset_sum() {
    For(300) {
        int mod = RNG(1, 200);
        int n = RNG(0, 15);
        vector<int> vals(n);
        For(i, n) vals[i] = RNG(0, mod);
        Modular_Subset_Sum<int> X(mod, vals);
        vector<bool> dp(mod);
        dp[0] = true;
        for (int v: vals) {
            vector<bool> ndp = dp;
            For(x, mod) if (dp[x]) ndp[(x + v) % mod] = true;
            dp = ndp;
        }
        For(x, mod) {
            assert(X[x] == dp[x]);
            if (!dp[x]) continue;
            auto I = X.restore(x);
            long long s = 0;
            for (int i: I) s += vals[i];
            // each value used at most once
            settify(I);
            assert(len(I) == len(X.restore(x)));
            assert(s % mod == x);
        }
    }
}

template <typename M>
void check_modfast() {
    auto X = make_unique<ModFast<M>>();
    const int p = M::get_mod();
    M r = X->root;
    For(100000) {
        M a = RNG(1, p);
        long long e = RNG(-(1LL << 62), 1LL << 62);
        assert(X->pow(a, e) == a.pow(e));
        unsigned int lg = X->log_r(a);
        assert(lg < 2u * (p - 1) && r.pow(lg) == a);
        assert(X->inverse(a) * a == M(1));
        assert(X->pow_r(e) == r.pow(e));
    }
    assert(X->pow(M(0), 0) == M(1) && X->pow(M(0), 5) == M(0));
    auto T = X->get_log_table(1000);
    For(x, 1, 1001) assert(r.pow(T[x]) == M(x));
}

void test_modfast() {
    check_modfast<modint<1000003>>();
    check_modfast<mint>();
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_modint();
    test_roots();
    test_u64();
    test_inverse();
    test_linear_mod();
    test_floor_sum_polynomial();
    test_binomials();
    test_convolution();
    test_subset_sum();
    test_modfast();
    solve();
    return 0;
}
