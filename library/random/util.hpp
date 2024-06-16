


namespace mitsuha{
unsigned long long rng() {
    static uint64_t x_
            = uint64_t(chrono::duration_cast<chrono::nanoseconds>(
                    chrono::high_resolution_clock::now().time_since_epoch())
                               .count())
              * 10150724397891781847ULL;
    x_ ^= x_ << 7;
    return x_ ^= x_ >> 9;
}

unsigned long long rand(unsigned long long lim) { return rng() % lim; }

long long rand(long long l, long long r) { return l + rng() % (r - l); }

pair<long long, long long> range(long long n){
    long long L = rand(0, n);
    long long R = rand(L + 1, n + 1);
    return make_pair(L, R);
}
} // namespace mitsuha