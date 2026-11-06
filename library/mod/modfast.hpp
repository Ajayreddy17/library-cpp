#ifndef AJAY_MODFAST
#define AJAY_MODFAST

#include "library/mod/modint.hpp"
#include "library/mod/primitive_root.hpp"
#include "library/number/lpf_table.hpp"
#include "library/datastructure/hashmap.hpp"
#include "library/random/base.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/modfast.hpp
// tables for a prime modint p < 2^30:
// O(1) pow, discrete log (base primitive root) and inverse
template <typename mint>
struct ModFast {
    static constexpr int LIM = 1 << 21;
    static constexpr int p = mint::get_mod();
    static_assert(2 <= p && p < (1 << 30));

    // small p: tabulate every nonzero element directly
    static constexpr bool DIRECT = (p <= LIM);
    static constexpr int K = (DIRECT ? p - 1 : LIM);

    static constexpr int TABLE_SIZE = (DIRECT ? p : 2 * K + 1);
    static constexpr int FRAC_SIZE = (DIRECT ? 1 : 1 + (1 << 20));

    unsigned int root;
    array<unsigned int, 32769> POW[2];
    array<pair<uint16_t, uint16_t>, FRAC_SIZE> FRAC;

    array<unsigned int, TABLE_SIZE> LOG;
    array<unsigned int, TABLE_SIZE> INV;

    ModFast() {
        root = (p == 998244353 ? 3 : primitive_root(p));
        build_pow();
        build_inv();
        build_log();
        if constexpr (!DIRECT) build_frac();
    }

    // a^exp; any signed exp when a != 0, exp >= 0 when a == 0
    mint pow(mint a, long long exp) const {
        if (a == 0) {
            assert(exp >= 0);
            return mint::raw(exp == 0);
        }

        unsigned int lg = log_r(a);

        // lg < 2^31, so the product fits in u64
        if ((unsigned long long)(exp) < (1ULL << 33)) {
            unsigned int e = (unsigned long long)(lg) * (unsigned long long)(exp) % (p - 1);
            return pow_r_32(e);
        }

        long long e = exp % (p - 1);
        if (e < 0) e += p - 1;
        unsigned int f = (unsigned long long)(lg) * (unsigned long long)(e) % (p - 1);
        return pow_r_32(f);
    }

    // root^exp
    mint pow_r(long long exp) const {
        exp %= p - 1;
        if (exp < 0) exp += p - 1;
        return pow_r_32(exp);
    }

    // 0 <= exp <= p - 1
    mint pow_r_32(unsigned int exp) const {
        assert(exp <= (unsigned int)(p - 1));
        return mint::raw((unsigned long long)(POW[0][exp & 32767]) * POW[1][exp >> 15] % p);
    }

    // e with x = root^e, in [0, 2p - 2) (reduce mod p - 1 if needed)
    unsigned int log_r(mint x) const {
        assert(x != 0);

        if constexpr (DIRECT) {
            return LOG[x.val];
        } else {
            auto [a, b] = FRAC[x.val >> 10];
            // t = xb - ap as a wrapped u32, |t| <= K
            unsigned int t = x.val * (unsigned int)(b) - (unsigned int)(a) * (unsigned int)(p);
            return LOG[K + t] + (p - 1) - LOG[K + b];
        }
    }

    mint inverse(mint x) const {
        assert(x != 0);

        if constexpr (DIRECT) {
            return mint::raw(INV[x.val]);
        } else {
            auto [a, b] = FRAC[x.val >> 10];
            unsigned int t = x.val * (unsigned int)(b) - (unsigned int)(a) * (unsigned int)(p);
            return mint::raw(INV[K + t] * (unsigned long long)(b) % p);
        }
    }

    // res[x] = log_r(x) for x <= n, res[0] = 0 (dummy)
    vector<unsigned int> get_log_table(int n) const {
        assert(0 <= n && n <= K);

        if constexpr (DIRECT) {
            return {LOG.begin(), LOG.begin() + n + 1};
        } else {
            return {LOG.begin() + K, LOG.begin() + K + n + 1};
        }
    }

private:
    void build_inv() {
        if constexpr (DIRECT) {
            INV[0] = 0;
            INV[1] = 1;

            for (unsigned int i = 2; i < (unsigned int)(p); ++i) {
                unsigned long long q = (p + i - 1) / i;
                unsigned int t = i * q - p;
                INV[i] = INV[t] * q % p;
            }
        } else {
            // INV[K + i] = i^{-1} for |i| <= K
            INV[K] = 0;
            INV[K + 1] = 1;

            for (unsigned int i = 2; i <= (unsigned int)K; ++i) {
                unsigned long long q = (p + i - 1) / i;
                INV[K + i] = INV[K + i * q - p] * q % p;
            }

            For(i, 1, K + 1) { INV[K - i] = p - INV[K + i]; }
        }
    }

    void build_pow() {
        POW[0][0] = POW[1][0] = 1;

        For(i, (1 << 15)) { POW[0][i + 1] = POW[0][i] * (unsigned long long)(root) % p; }
        For(i, (1 << 15)) { POW[1][i + 1] = POW[1][i] * (unsigned long long)(POW[0][1 << 15]) % p; }
    }

    void build_log() {
        if constexpr (DIRECT) {
            LOG[0] = 0;

            unsigned int x = 1;
            For(e, p - 1) {
                LOG[x] = e;
                x = (unsigned long long)(x) * root % p;
            }

            return;
        }

        // LOG[K + i] = log i for |i| <= K
        auto lpf = lpf_table(K);

        const int S = 1 << 17;
        HashMap<unsigned int> MP(S);

        unsigned int pw = 1;
        for (int k = 0; k < S; ++k, pw = (unsigned long long)(root) * pw % p) { MP[pw] = k; }

        unsigned int q = pow_r_32(p - 1 - S).val;

        auto BSGS = [&](unsigned int s) -> unsigned int {
            unsigned int ans = 0;
            while (1) {
                unsigned int v = MP.get(s, -1);
                if (v != (unsigned int)(-1)) return ans + v;
                ans += S;
                s = (unsigned long long)(s) * q % p;
            }
            return 0;
        };

        LOG[K] = 0;
        LOG[K + 1] = 0;

        For(i, 2, K + 1) {
            // composite: sum of logs of factors
            if (lpf[i] < i) {
                LOG[K + i] = (LOG[K + lpf[i]] + LOG[K + i / lpf[i]]) % (p - 1);
                continue;
            }

            if (i < 100) {
                LOG[K + i] = BSGS(i);
                continue;
            }

            if (i * i > p) {
                auto [j, k] = divmod<int>(p, i);
                // i = (-k) / j
                LOG[K + i] = (LOG[K + k] + (p - 1) / 2 + (p - 1) - LOG[K + j]) % (p - 1);
                continue;
            }

            // index calculus: i * root^k smooth over primes < i
            while (1) {
                unsigned int k = RNG(0, p - 1);
                unsigned long long ans = p - 1 - k;
                unsigned int x = (unsigned long long)(i) * pow_r_32(k).val % p;

                auto div = [&](unsigned int q) -> void {
                    x /= q;
                    ans += LOG[K + q];
                };

                for (unsigned int q : {2, 3, 5, 7, 11, 13, 17, 19}) {
                    while (x % q == 0) div(q);
                }

                if (x >= (unsigned int)K) continue;

                while ((unsigned int)i < x && x < (unsigned int)K && lpf[x] < i) { div(lpf[x]); }
                if (1 < x && x < (unsigned int)i) div(x);

                if (x == 1) {
                    LOG[K + i] = ans % (p - 1);
                    break;
                }
            }
        }

        For(i, 1, K + 1) { LOG[K - i] = (LOG[K + i] + (p - 1) / 2) % (p - 1); }
    }

    // FRAC[s] = a/b with b < 2048 close to s * 1024 / p
    void build_frac() {
        static_assert(!DIRECT);

        vector<tuple<uint16_t, uint16_t, uint16_t, uint16_t>> que;
        que.emplace_back(0, 1, 1, 1);

        while (len(que)) {
            auto [a, b, c, d] = POP(que);

            if (b + d < 2048) {
                que.emplace_back(a + c, b + d, c, d);
                que.emplace_back(a, b, a + c, b + d);
                continue;
            }

            unsigned int s = ((unsigned long long)(a) * p) / (1024 * b);
            unsigned int t = ((unsigned long long)(c) * p) / (1024 * d);

            FRAC[s] = {a, b};
            FRAC[t] = {c, d};

            a = min(a, c);
            b = min(b, d);
            For(i, s + 1, t) FRAC[i] = {a, b};
        }
    }
};
} // namespace mitsuha
#endif // AJAY_MODFAST
