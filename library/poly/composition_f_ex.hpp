#ifndef AJAY_COMPOSITION_F_EX
#define AJAY_COMPOSITION_F_EX

#include "library/poly/sum_of_exp_bx.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/composition_f_ex.hpp

// f(e^x) up to degree N for a polynomial f of degree N. O(Nlog^2N)
// if f is longer than N but only [0,N] is wanted, resize(N+1)
// changes the answer, so be careful
template <typename mint>
vector<mint> composition_f_ex(vector<mint> f) {
    int N = len(f) - 1;
    vector<mint> A, B;
    For(k, len(f)) A.emplace_back(f[k]), B.emplace_back(mint(k));
    return sum_of_exp_bx(N, A, B);
}
} // namespace mitsuha
#endif // AJAY_COMPOSITION_F_EX
