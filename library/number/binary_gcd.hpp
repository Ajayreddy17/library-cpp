#ifndef AJAY_BINARY_GCD
#define AJAY_BINARY_GCD

namespace mitsuha{
namespace BinaryGCDImpl {
    unsigned long long binary_gcd(unsigned long long a, unsigned long long b) {
        if (a == 0 || b == 0) return a + b;
        char n = __builtin_ctzll(a);
        char m = __builtin_ctzll(b);
        a >>= n;
        b >>= m;
        n = min(n, m);
        while (a != b) {
            unsigned long long d = a - b;
            char s = __builtin_ctzll(d);
            bool f = a > b;
            b = f ? b : a;
            a = (f ? d : -d) >> s;
        }
        return a << n;
    }
    
    // a > 0
    int ctz128(__uint128_t a) {
        unsigned long long lo = a & (unsigned long long)(-1);
        return lo ? __builtin_ctzll(lo) : 64 + __builtin_ctzll(a >> 64);
    }
    __uint128_t binary_gcd128(__uint128_t a, __uint128_t b) {
        if (a == 0 || b == 0) return a + b;
        char n = ctz128(a);
        char m = ctz128(b);
        a >>= n;
        b >>= m;
        n = min(n, m);
        while (a != b) {
            __uint128_t d = a - b;
            char s = ctz128(d);
            bool f = a > b;
            b = f ? b : a;
            a = (f ? d : -d) >> s;
        }
        return a << n;
    }

}  // namespace BinaryGCDImpl

long long bgcd(long long a, long long b) {
    return BinaryGCDImpl::binary_gcd(abs(a), abs(b));
}
__int128_t bgcd128(__int128_t a, __int128_t b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    return BinaryGCDImpl::binary_gcd128(a, b);
}
} // namespace mitsuha
#endif // AJAY_BINARY_GCD