#ifndef AJAY_HADAMARD
#define AJAY_HADAMARD

#include "library/util/bit_utils.hpp"

namespace mitsuha {
// B[j] = sum_i (-1)^{popcnt(i&j)A[i]}
// 2 ^ n Division is not being done
template <typename T>
void hadamard(vector<T>& A) {
    int log = msb(int(A.size()));
    assert(1 << log == int(A.size()));
    For(n, log) For(s, 1 << log) {
        int t = s ^ (1 << n);
        if (s < t) tie(A[s], A[t]) = make_pair(A[s] + A[t], A[s] - A[t]);
    }
}
}
#endif // AJAY_HADAMARD