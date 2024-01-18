#ifndef AJAY_PROD_ONE_PLUS_XN
#define AJAY_PROD_ONE_PLUS_XN

#include "library/poly/fps_exp.hpp"

namespace mitsuha{
// Find prod (1 + x^a) up to x^{LIM}
template <typename mint, typename INT>
vector<mint> product_of_one_plus_xn(vector<INT> &A, int LIM) {
    vector<int> CNT(LIM + 1);
    for (auto&& x: A) {
        if (x <= LIM) CNT[x]++;
    }
    
    vector<mint> logf(LIM + 1);
    For(x, 1, LIM + 1) {
        For(d, 1, LIM / x + 1) {
            if (d & 1)
                logf[d * x] += mint(CNT[x]) * inv<mint>(d);
            else
                logf[d * x] -= mint(CNT[x]) * inv<mint>(d);
        }
    }
    auto f = fps_exp(logf);
    mint c = mint(2).pow(CNT[0]);
    for (auto&& x: f) x *= c;
    return f;
}
} // namespace mitsuha
#endif // AJAY_PROD_ONE_PLUS_XN