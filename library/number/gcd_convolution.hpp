#ifndef AJAY_GCD_CONVOLUTION
#define AJAY_GCD_CONVOLUTION

#include "library/number/zeta.hpp"

namespace mitsuha{
template <typename T>
vector<T> gcd_convolution(vector<T> A, vector<T> B) {
    assert(len(A) == len(B));
    multiplier_zeta(A);
    multiplier_zeta(B);
    For(i, len(A)) A[i] *= B[i];
    multiplier_mobius(A);
    return A;
}
} // namespace mitsuha
#endif // AJAY_GCD_CONVOLUTION