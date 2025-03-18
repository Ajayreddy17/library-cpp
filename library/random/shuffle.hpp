#ifndef AJAY_SHUFFLE
#define AJAY_SHUFFLE

#include "library/random/base.hpp"

namespace mitsuha {
template <typename T>
void shuffle(vector<T>& A) {
    For(i, len(A)) {
        int j = RNG(0, i + 1);
        if (i != j) swap(A[i], A[j]);
    }
}
} // namespace mitsuha
#endif // AJAY_SHUFFLE