#ifndef AJAY_XOR_CONVOLUTION
#define AJAY_XOR_CONVOLUTION

#include "library/setfunc/hadamard.hpp"

namespace mitsuha{
template <typename T>
vector<T> xor_convolution(vector<T> A, vector<T> B) {
    hadamard(A);
    hadamard(B);
    For(i, int(A.size())) A[i] *= B[i];
    hadamard(A);

    T c = T(1) / T(int(A.size()));
    if (c != T(0)) {
        For(i, int(A.size())) A[i] *= c;
    } else {
        For(i, int(A.size())) A[i] /= int(A.size());
    }
    return A;
}
} // namespace mitsuha
#endif // AJAY_XOR_CONVOLUTION