#ifndef AJAY_LPF_TABLE
#define AJAY_LPF_TABLE

#include "library/number/primetable.hpp"

namespace mitsuha{
vector<int> lpf_table(long long LIM) {
    auto primes = primetable(LIM);
    vector<int> res(LIM + 1, -1);
    Frr(i, len(primes)) {
        auto p = primes[i];
        For(j, 1, LIM / p + 1) res[p * j] = p;
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_LPF_TABLE