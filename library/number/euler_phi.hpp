#ifndef AJAY_EULER_PHI
#define AJAY_EULAR_PHI

#include "library/number/zeta.hpp"
#include "library/number/factor.hpp"

namespace mitsuha{
long long euler_phi(long long n) {
    auto pf = factor(n);
    for (auto&& [p, e]: pf) n -= n / p;
    return n;
}

template <typename T>
vector<T> euler_phi_table(long long n) {
    vector<T> A(n + 1);
    For(i, 1, n + 1) A[i] = T(i);
    divisor_mobius(A);
    return A;
}
} // namespace mitsuha
#endif // AJAY_EULER_PHI