#ifndef AJAY_BIGINT_BASE
#define AJAY_BIGINT_BASE

#include "library/poly/convolution.hpp"

namespace mitsuha{
// Separate by 10^9
struct BigInteger {
    static constexpr int TEN[]
            = {1,      10,      100,      1000,      10000,
               100000, 1000000, 10000000, 100000000, 1000000000};
    static constexpr int LOG = 9;
    static constexpr int MOD = TEN[LOG];
    using bint = BigInteger;
    int sgn; // +1 or -1. -0 is allowed in the internal state.
    vector<int> dat;

    BigInteger() : sgn(1) {}
    BigInteger(__int128_t val) {
        sgn = 1;
        if (val != 0) {
            if (val < 0) sgn = -1, val = -val;
            while (val > 0) {
                dat.emplace_back(val % MOD);
                val /= MOD;
            }
        }
    }
    BigInteger(string s) {
        assert(!s.empty());
        sgn = 1;
        if (s[0] == '-') {
            sgn = -1;
            s.erase(s.begin());
            assert(!s.empty());
        }
        if (s[0] == '0') s.clear();
        reverse(s.begin(), s.end());
        int n = len(s);
        int m = cld(n, LOG);
        dat.assign(m, 0);
        For(i, n) { dat[i / LOG] += TEN[i % LOG] * (s[i] - '0'); }
    }
    bint &operator=(const bint &p) {
        sgn = p.sgn;
        dat = p.dat;
        return *this;
    }
    bool operator<(const bint &p) const {
        if (sgn != p.sgn) {
            if (dat.empty() && p.dat.empty()) return false;
            return sgn < p.sgn;
        }
        if (len(dat) != len(p.dat)) {
            if (sgn == 1) return len(dat) < len(p.dat);
            if (sgn == -1) return len(dat) > len(p.dat);
        }
        Frr(i, len(dat)) {
            if (dat[i] == p.dat[i]) continue;
            if (sgn == 1) return dat[i] < p.dat[i];
            if (sgn == -1) return dat[i] > p.dat[i];
        }
        return false;
    }
    bool operator>(const bint &p) const { return p < *this; }
    bool operator<=(const bint &p) const { return !(*this > p); }
    bool operator>=(const bint &p) const { return !(*this < p); }
    bint &operator+=(const bint p) {
        if (sgn != p.sgn) {
            *this -= (-p);
            return *this;
        }
        int n = max(len(dat), len(p.dat));
        dat.resize(n + 1);
        For(i, n) {
            if (i < len(p.dat)) dat[i] += p.dat[i];
            if (dat[i] >= MOD) dat[i] -= MOD, dat[i + 1] += 1;
        }
        while (len(dat) && dat.back() == 0) dat.pop_back();
        return *this;
    }
    bint &operator-=(const bint p) {
        if (sgn != p.sgn) {
            *this += (-p);
            return *this;
        }
        if ((sgn == 1 && *this < p) || (sgn == -1 && *this > p)) {
            *this = p - *this;
            sgn = -sgn;
            return *this;
        }
        For(i, len(p.dat)) { dat[i] -= p.dat[i]; }
        For(i, len(dat) - 1) {
            if (dat[i] < 0) dat[i] += MOD, dat[i + 1] -= 1;
        }
        while (len(dat) && dat.back() == 0) { dat.pop_back(); }
        return *this;
    }
    bint &operator*=(const bint &p) {
        sgn *= p.sgn;
        dat = convolve(dat, p.dat);
        return *this;
    }
    // bint &operator/=(const bint &p) { return *this; }
    bint operator-() const {
        bint p = *this;
        p.sgn *= -1;
        return p;
    }
    bint operator+(const bint &p) const { return bint(*this) += p; }
    bint operator-(const bint &p) const { return bint(*this) -= p; }
    bint operator*(const bint &p) const { return bint(*this) *= p; }
    // bint operator/(const modint &p) const { return modint(*this) /= p; }
    bool operator==(const bint &p) const {
        if (dat.empty() && p.dat.empty()) return true;
        return (sgn == p.sgn && dat == p.dat);
    }
    bool operator!=(const bint &p) const { return !((*this) == p); }

    vector<int> convolve(const vector<int> &a, const vector<int> &b) {
        int n = len(a), m = len(b);
        if (!n || !m) return {};
        if (min(n, m) <= 500) {
            vector<int> c(n + m - 1);
            __uint128_t x = 0;
            For(k, n + m - 1) {
                int s = max<int>(0, k + 1 - m), t = min<int>(k, n - 1);
                For(i, s, t + 1) { x += (unsigned long long)(a[i]) * b[k - i]; }
                c[k] = x % MOD, x = x / MOD;
            }
            while (x > 0) { c.emplace_back(x % MOD), x = x / MOD; }
            return c;
        }
        static constexpr int p0 = 167772161;
        static constexpr int p1 = 469762049;
        static constexpr int p2 = 754974721;
        using mint0 = modint<p0>;
        using mint1 = modint<p1>;
        using mint2 = modint<p2>;
        vector<mint0> a0(a.begin(), a.end()), b0(b.begin(), b.end());
        vector<mint1> a1(a.begin(), a.end()), b1(b.begin(), b.end());
        vector<mint2> a2(a.begin(), a.end()), b2(b.begin(), b.end());
        auto c0 = convolution_ntt<mint0>(a0, b0);
        auto c1 = convolution_ntt<mint1>(a1, b1);
        auto c2 = convolution_ntt<mint2>(a2, b2);
        vector<int> c(len(c0));
        __uint128_t x = 0;
        For(i, n + m - 1) {
            x += CRT3<__uint128_t, p0, p1, p2>(c0[i].val, c1[i].val, c2[i].val);
            c[i] = x % MOD, x = x / MOD;
        }
        while (x) { c.emplace_back(x % MOD), x = x / MOD; }
        return c;
    }

    string to_string() {
        if (dat.empty()) return "0";
        string s;
        for (int x: dat) {
            For(LOG) {
                s += '0' + (x % 10);
                x = x / 10;
            }
        }
        while (s.back() == '0') s.pop_back();
        if (sgn == -1) s += '-';
        reverse(s.begin(), s.end());
        return s;
    }

    // https://codeforces.com/contest/504/problem/D
    string to_binary_string() {
        vector<unsigned int> A(dat.begin(), dat.end());
        string ANS;
        while (1) {
            while (len(A) && A.back() == (unsigned int)0) A.pop_back();
            if (A.empty()) break;
            unsigned long long rem = 0;
            Frr(i, len(A)) {
                rem = rem * MOD + A[i];
                A[i] = rem >> 32;
                rem &= (unsigned int)(-1);
            }
            For(i, 32) { ANS += '0' + (rem >> i & 1); }
        }
        while (len(ANS) && ANS.back() == '0') ANS.pop_back();
        reverse(ANS.begin(), ANS.end());
        if (ANS.empty()) ANS += '0';
        return ANS;
    }

    // https://codeforces.com/contest/759/problem/E
    pair<bint, int> divmod(int p) {
        vector<int> after;
        long long rm = 0;
        Frr(i, len(dat)) {
            rm = rm * MOD + dat[i];
            after.emplace_back(rm / p);
            rm = rm % p;
        }
        reverse(after.begin(), after.end());
        while (len(after) && after.back() == 0) after.pop_back();
        bint q;
        q.sgn = sgn;
        q.dat = after;
        rm *= sgn;
        if (rm < 0) {
            rm += p;
            q -= 1;
        }
        return {q, rm};
    }

    // https://codeforces.com/problemset/problem/582/D
    vector<int> base_p_representation(int p) {
        vector<unsigned int> A(dat.begin(), dat.end());
        vector<int> res;
        while (1) {
            while (len(A) && A.back() == (unsigned int)(0)) A.pop_back();
            if (A.empty()) break;
            unsigned long long rm = 0;
            Frr(i, len(A)) {
                rm = rm * MOD + A[i];
                A[i] = rm / p;
                rm %= p;
            }
            res.emplace_back(rm);
        }
        reverse(res.begin(), res.end());
        return res;
    }

    // Calculate by ignoring overflow
    long long to_ll() {
        long long x = 0;
        Frr(i, len(dat)) x = MOD * x + dat[i];
        return sgn * x;
    }

    // https://codeforces.com/contest/986/problem/D
    bint pow(long long n) {
        auto dfs = [&](auto &dfs, long long n) -> bint {
            if (n == 1) return (*this);
            bint x = dfs(dfs, n / 2);
            x *= x;
            if (n & 1) x *= (*this);
            return x;
        };
        if (n == 0) return bint(1);
        return dfs(dfs, n);
    }

    // https://codeforces.com/contest/986/problem/D
    double log10() {
        assert(!dat.empty() && sgn == 1);
        if (len(dat) <= 3) {
            double x = 0;
            Frr(i, len(dat)) x = MOD * x + dat[i];
            return std::log10(x);
        }
        double x = 0;
        Frr(i, 4) x = MOD * x + dat[len(dat) - 1 - i];
        x = std::log10(x);
        x += double(LOG) * (len(dat) - 4);
        return x;
    }
};

#ifdef FASTIO
void wt(BigInteger x) { io::wt(x.to_string()); }
void rd(BigInteger &x) {
  string s;
  io::rd(s);
  x = BigInteger(s);
}
#endif

ostream &operator<<(ostream &out, const BigInteger &x){
    auto X = x;
    return out << X.to_string();
}
} // namespace mitsuha
#endif // AJAY_BIGINT_BASE
