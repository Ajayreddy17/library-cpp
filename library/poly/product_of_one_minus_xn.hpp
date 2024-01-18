#ifndef AJAY_PROD_ONE_MINUS_XN
#define AJAY_PROD_ONE_MINUS_XN

#include "library/poly/fps_exp.hpp"

namespace mitsuha{
// Find prod (1 - x^a) up to x^{LIM}
template <typename mint, typename INT>
vector<mint> product_of_one_minus_xn(vector<INT> &A, int LIM) {
    vector<int> CNT(LIM + 1);
    for (auto&& x: A) {
        if (x <= LIM) CNT[x]++;
    }
    if (CNT[0]) { return vc<mint>(LIM + 1); }

    vector<mint> logf(LIM + 1);
    For(x, 1, LIM + 1) {
        For(d, 1, LIM / x + 1) { logf[d * x] -= mint(CNT[x]) * inv<mint>(d); }
    }
    return fps_exp(logf);
}
} // namespace mitsuha
#endif // AJAY_PROD_ONE_MINUS_XN