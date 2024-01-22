#ifndef AJAY_BARRETT
#define AJAY_BARRETT

namespace mitsuha{
// https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp
struct Barrett {
    unsigned int m;
    unsigned long long im;
    explicit Barrett(unsigned int m = 1) : m(m), im((unsigned long long)(-1) / m + 1) {}
    unsigned int umod() const { return m; }
    unsigned int modulo(unsigned long long z) {
        if (m == 1) return 0;
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * m;
        return (z - y + (z < y ? m : 0));
    }
    unsigned long long floor(unsigned long long z) {
        if (m == 1) return z;
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * m;
        return (z < y ? x - 1 : x);
    }
    pair<unsigned long long, unsigned int> divmod(unsigned long long z) {
        if (m == 1) return {z, 0};
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * m;
        if (z < y) return {x - 1, z - y + m};
        return {x, z - y};
    }
    unsigned int mul(unsigned int a, unsigned int b) { return modulo((unsigned long long)(a) * b); }
};

struct Barrett_64 {
    unsigned __int128 mod, mh, ml;

    explicit Barrett_64(unsigned long long mod = 1) : mod(mod) {
        unsigned __int128 m = (unsigned __int128)(-1) / mod;
        if (m * mod + mod == (unsigned __int128)(0)) ++m;
        mh = m >> 64;
        ml = m & (unsigned long long)(-1);
    }

    unsigned long long umod() const { return mod; }

    unsigned long long modulo(unsigned __int128 x) {
        unsigned __int128 z = (x & (unsigned long long)(-1)) * ml;
        z = (x & (unsigned long long)(-1)) * mh + (x >> 64) * ml + (z >> 64);
        z = (x >> 64) * mh + (z >> 64);
        x -= z * mod;
        return x < mod ? x : x - mod;
    }

    unsigned long long mul(unsigned long long a, unsigned long long b) { return modulo((unsigned __int128)(a) * b); }
};
} // namespace mitsuha
#endif // AJAY_BARRETT