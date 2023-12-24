#ifndef AJAY_RANDOM
#define AJAY_RANDOM

namespace mitsuha{
unsigned long long RNG_64() {
    static uint64_t x_
            = uint64_t(chrono::duration_cast<chrono::nanoseconds>(
                    chrono::high_resolution_clock::now().time_since_epoch())
                               .count())
              * 10150724397891781847ULL;
    x_ ^= x_ << 7;
    return x_ ^= x_ >> 9;
}

unsigned long long RNG(unsigned long long lim) { return RNG_64() % lim; }

long long RNG(long long l, long long r) { return l + RNG_64() % (r - l); }
} // namespace mitsuha
#endif // AJAY_RANDOM
