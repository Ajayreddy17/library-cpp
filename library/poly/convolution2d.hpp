#ifndef AJAY_CONVOLUTION2D
#define AJAY_CONVOLUTION2D

#include "library/poly/convolution.hpp"

namespace mitsuha{
template <typename T>
vector<vector<T>> convolution2d(vector<vector<T>>& f, vector<vector<T>>& g) {
    auto shape = [&](vector<vector<T>>& f) -> pair<long long, long long> {
        long long H = len(f);
        long long W = (H == 0 ? 0 : len(f[0]));
        return {H, W};
    };
    auto [H1, W1] = shape(f);
    auto [H2, W2] = shape(g);
    long long H = H1 + H2 - 1;
    long long W = W1 + W2 - 1;

    vector<T> ff(H1 * W);
    vector<T> gg(H2 * W);
    For(x, H1) For(y, W1) ff[W * x + y] = f[x][y];
    For(x, H2) For(y, W2) gg[W * x + y] = g[x][y];
    auto hh = convolution(ff, gg);
    vector<vector<T>> h(H, vector<T>(W));
    For(x, H) For(y, W) h[x][y] = hh[W * x + y];
    return h;
}
} // namespace mitsuha
#endif // AJAY_CONVOLUTION2D