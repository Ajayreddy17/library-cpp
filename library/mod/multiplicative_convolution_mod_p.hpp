#ifndef AJAY_MULTIPLICATIVE_CONVOLUTION_MOD_P
#define AJAY_MULTIPLICATIVE_CONVOLUTION_MOD_P

#include "library/poly/convolution.hpp"
#include "library/mod/primitive_root.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/multiplicative_convolution_mod_p.hpp
// C[k] = sum_{ij = k mod p} A[i] B[j], p prime, len(A) = len(B) = p
// nonzero residues: index by discrete log base a primitive root -> cyclic convolution
template <typename mint>
vector<mint> multiplicative_convolution_mod_p(int p, vector<mint>& A, vector<mint>& B) {
    assert(len(A) == p && len(B) == p);
    int r = primitive_root(p);
    vector<int> exp(p - 1), log(p);
    exp[0] = 1;
    For(i, p - 2) exp[i + 1] = exp[i] * (long long)(r) % p;
    For(i, p - 1) log[exp[i]] = i;

    vector<mint> FA(p - 1), FB(p - 1);
    For(i, p - 1) FA[i] = A[exp[i]], FB[i] = B[exp[i]];
    vector<mint> FC = convolution(FA, FB);
    For(i, p - 1, len(FC)) FC[i - (p - 1)] += FC[i];

    vector<mint> C(p);
    For(i, 1, p) C[i] = FC[log[i]];

    mint SA = accumulate(A.begin(), A.end(), mint(0)), SB = accumulate(B.begin(), B.end(), mint(0));
    C[0] += SA * B[0] + SB * A[0] - A[0] * B[0];
    return C;
}
} // namespace mitsuha
#endif // AJAY_MULTIPLICATIVE_CONVOLUTION_MOD_P
