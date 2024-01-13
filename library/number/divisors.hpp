#ifndef AJAY_DIVISORS
#define AJAY_DIVISORS

#include "library/number/factor.hpp"

namespace mitsuha{
// Does not sort
vector<long long> divisors_by_pf(const vector<pair<long long, int>>& pf) {
    vector<long long> div = {1};
    for (auto&& [p, e]: pf) {
        long long n = len(div);
        long long pp = 1;
        For(i, 1, e + 1) {
            pp *= p;
            For(j, n) div.emplace_back(div[j] * pp);
        }
    }
    return div;
}

// Does not sort
vector<long long> divisors(long long N) {
    auto pf = factor(N);
    return divisors_by_pf(pf);
}

// Does not sort
vector<long long> divisors_by_lpf(long long N, vector<int>& lpf) {
    auto pf = factor_by_lpf(N, lpf);
    return divisors_by_pf(pf);
}
} // namespace mitsuha
#endif // AJAY_DIVISORS