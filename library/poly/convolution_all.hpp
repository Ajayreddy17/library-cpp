#ifndef AJAY_CONVOLUTION_ALL
#define AJAY_CONVOLUTION_ALL

#include "library/poly/convolution.hpp"

namespace mitsuha{
template <typename T>
vector<T> convolution_all(vector<vector<T>>& polys) {
    if (len(polys) == 0) return {T(1)};
    while (1) {
        int n = len(polys);
        if (n == 1) break;
        int m = ceil(n, 2);
        For(i, m) {
            if (2 * i + 1 == n) {
                polys[i] = polys[2 * i];
            } else {
                polys[i] = convolution(polys[2 * i], polys[2 * i + 1]);
            }
        }
        polys.resize(m);
    }
    return polys[0];
}
} // namespace mitsuha
#endif // AJAY_CONVOLUTION_ALL