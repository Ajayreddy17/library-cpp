#ifndef AJAY_PRIMETABLE
#define AJAY_PRIMETABLE

namespace mitsuha{
// returns vector of primes <= LIM
template <typename T = int>
vector<T> primetable(int LIM) {
    ++LIM;
    const int S = 32768;
    static int done = 2;
    static vector<T> primes = {2}, sieve(S + 1);

    if (done < LIM) {
        done = LIM;

        primes = {2}, sieve.assign(S + 1, 0);
        const int R = LIM / 2;
        primes.reserve(int(LIM / log(LIM) * 1.1));
        vector<pair<int, int>> cp;
        for (int i = 3; i <= S; i += 2) {
            if (!sieve[i]) {
                cp.emplace_back(i, i * i / 2);
                for (int j = i * i; j <= S; j += 2 * i) sieve[j] = 1;
            }
        }
        for (int L = 1; L <= R; L += S) {
            array<bool, S> block{};
            for (auto& [p, idx]: cp)
                for (int i = idx; i < S + L; idx = (i += p)) block[i - L] = 1;
            For(i, min(S, R - L)) if (!block[i]) primes.emplace_back((L + i) * 2 + 1);
        }
    }
    int k = lower_bound(primes.begin(), primes.end(), LIM + 1) - primes.begin();
    return {primes.begin(), primes.begin() + k};
}
} // namespace mitsuha
#endif // AJAY_PRIMETABLE