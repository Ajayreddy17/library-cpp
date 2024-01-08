#ifndef AJAY_MOD_INV
#define AJAY_MOD_INV

namespace mitsuha{
// Long is okay
// Make sure (val * x - 1) is a multiple of mod
// Especially if mod=0, x=0 satisfies
long long mod_inv(long long val, long long mod) {
    if (mod == 0) return 0;
    mod = abs(mod);
    val %= mod;
    if (val < 0) val += mod;
    long long a = val, b = mod, u = 1, v = 0, t;
    while (b > 0) {
        t = a / b;
        swap(a -= t * b, b), swap(u -= t * v, v);
    }
    if (u < 0) u += mod;
    return u;
}
} // namespace mitsuha
#endif // AJAY_MOD_INV