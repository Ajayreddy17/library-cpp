#ifndef AJAY_MOD_INV_U64
#define AJAY_MOD_INV_U64

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/mod_inv_u64.hpp
// a^{-1} mod 2^64 for odd a (Newton iteration, precision doubles each step)
unsigned long long mod_inv_u64(unsigned long long a) {
    assert(a & 1);
    unsigned long long x = 1;
    For(6) x = x * (2ULL - a * x);
    assert(a * x == 1);
    return x;
}
} // namespace mitsuha
#endif // AJAY_MOD_INV_U64
