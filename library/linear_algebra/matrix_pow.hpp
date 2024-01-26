#ifndef AJAY_MATRIX_POW
#define AJAY_MATRIX_POW

#include "library/linear_algebra/matrix_mul.hpp"

namespace mitsuha{
template <typename T>
vector<vector<T>> matrix_pow(vector<vector<T>> A, long long n) {
    int N = len(A);
    vector<vector<T>> ret(N, vector<T>(N));
    For(i, N) ret[i][i] = T(1);
    while (n) {
        if (n & 1) ret = matrix_mul(ret, A, N, N, N);
        n /= 2;
        if (n) A = matrix_mul(A, A, N, N, N);
    }
    return ret;
}

template <typename T, int N>
array<array<T, N>, N> matrix_pow(array<array<T, N>, N> A, long long n) {
    array<array<T, N>, N> ret{};
    For(i, N) ret[i][i] = T(1);
    while (n) {
        if (n & 1) ret = matrix_mul<T, N>(ret, A);
        n /= 2;
        if (n) A = matrix_mul<T, N>(A, A);
    }
    return ret;
}
} // namespace mitsuha
#endif // AJAY_MATRIX_POW