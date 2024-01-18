#ifndef AJAY_KTH_ROOT
#define AJAY_KTH_ROOT

namespace mitsuha{
unsigned long long integer_kth_root(unsigned long long k, unsigned long long a) {
    assert(k >= 1);
    if (a == 0 || a == 1 || k == 1) return a;
    if (k >= 64) return 1;
    if (k == 2) return sqrtl(a);
    if (a == (unsigned long long)(-1)) --a;
    struct S {
        unsigned long long v;
        S& operator*=(const S& o) {
            v = v <= (unsigned long long)(-1) / o.v ? v * o.v : (unsigned long long)(-1);
            return *this;
        }
    };
    auto power = [&](S x, long long n) -> S {
        S v{1};
        while (n) {
            if (n & 1) v *= x;
            x *= x;
            n /= 2;
        }
        return v;
    };
    unsigned long long res = pow(a, nextafter(1 / double(k), 0));
    while (power(S{res + 1}, k).v <= a) ++res;
    return res;
}
} // namespace mitsuha
#endif // AJAY_KTH_ROOT
