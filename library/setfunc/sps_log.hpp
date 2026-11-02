#ifndef AJAY_SPS_LOG
#define AJAY_SPS_LOG

#include "library/mod/modint.hpp"
#include "library/setfunc/sps_composition.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/setfunc/sps_log.hpp

// computed by reversing the steps of exp
template <typename mint, int LIM>
vector<mint> sps_log(vector<mint>& dp) {
    const int N = msb(len(dp));
    assert(len(dp) == (1 << N) && dp[0] == mint(1));
    vector<mint> s(1 << N);
    Frr(i, N) {
        vector<mint> a = {dp.begin() + (1 << i), dp.begin() + (2 << i)};
        vector<mint> b = {dp.begin(), dp.begin() + (1 << i)};
        auto RA = ranked_zeta<mint, LIM>(a);
        auto RB = ranked_zeta<mint, LIM>(b);
        For(s, 1 << i) {
            auto &f = RA[s], &g = RB[s];
            // assert(g[0] == mint(1));
            For(d, i + 1) { For(i, d) f[d] -= f[i] * g[d - i]; }
        }
        a = ranked_mobius<mint, LIM>(RA);
        copy(a.begin(), a.end(), s.begin() + (1 << i));
    }
    return s;
}
} // namespace mitsuha
#endif // AJAY_SPS_LOG
