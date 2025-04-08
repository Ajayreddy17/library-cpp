#ifndef AJAY_XOR_RANGE
#define AJAY_XOR_RANGE

namespace mitsuha{
template <typename F>
void xor_range(unsigned long long a, unsigned long long lo, unsigned long long hi, F query){
    For(k, 64){
        if (lo == hi) break;
        long long b = 1LL << k;
        if (lo & b) { query((lo ^ a), (lo ^ a) + b), lo += b; }
        if (hi & b) { query((hi - b) ^ a, ((hi - b) ^ a) + b), hi -= b; }
        if (a & b) a ^= b;
    }
}
} // namespace mitsuha
#endif // AJAY_XOR_RANGE