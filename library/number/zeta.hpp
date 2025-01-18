#ifndef AJAY_ZETA
#define AJAY_ZETA

#include "library/number/primetable.hpp"

namespace mitsuha {
template <typename T>
void divisor_zeta(vector<T>& A) {
    assert(A[0] == 0);
    int N = len(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) For(x, 1, N / p + 1) A[p * x] += A[x];
}

template <typename T>
void divisor_mobius(vector<T>& A) {
    assert(A[0] == 0);
    int N = len(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) Frr(x, 1, N / p + 1) A[p * x] -= A[x];
}

template <typename T>
void multiplier_zeta(vector<T>& A) {
    assert(A[0] == 0);
    int N = len(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) Frr(x, 1, N / p + 1) A[x] += A[p * x];
}

template <typename T>
void multiplier_mobius(vector<T>& A) {
    assert(A[0] == 0);
    int N = len(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) For(x, 1, N / p + 1) A[x] -= A[p * x];
}
} // namespace mitsuha
#endif // AJAY_ZETA