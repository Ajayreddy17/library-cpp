#ifndef AJAY_COEF_OF_FPS_POWS
#define AJAY_COEF_OF_FPS_POWS

#include "library/poly/convolution.hpp"
#include "library/poly/fps_div.hpp"
#include "library/poly/fps_pow.hpp"

namespace mitsuha{
// https://noshi91.hatenablog.com/entry/2024/03/16/224034
// k fix it [x^k]f(x)^ig(x) を i=0,1,...,n Find it with.
template <typename mint>
vector<mint> coef_of_fps_pows(vector<mint> f, int k, int n, vector<mint> g = {mint(1)}) {
    assert(len(f) == k + 1);
    vector<vector<mint>> P(k + 1, vector<mint>(2));
    vector<vector<mint>> Q(k + 1, vector<mint>(2));
    Q[0][0] = 1;
    For(i, len(g)) if (i <= k) P[i][0] = g[i];
    For(i, len(f)) if (i <= k) Q[i][1] = -f[i];
    For(i, len(f)) Q[i][1] = -f[i];

    while (k > 0) {
        int H = len(P), W = len(P[0]);
        vector<vector<mint>> R = Q;
        For(i, H) {
            if (i % 2 == 0) continue;
            For(j, W){ R[i][j] = -R[i][j]; }
        }
        vector<vector<mint>> F = convolution2d<mint>(P, R);
        vector<vector<mint>> G = convolution2d<mint>(Q, R);
        P.resize(k / 2 + 1);
        Q.resize(k / 2 + 1);
        For(i, len(P)) P[i] = F[2 * i | (k & 1)];
        For(i, len(Q)) Q[i] = G[2 * i];
        k /= 2;
        For(i, len(P)) if (len(P[i]) > n + 1) P[i].resize(n + 1);
        For(i, len(Q)) if (len(Q[i]) > n + 1) Q[i].resize(n + 1);
    }
    vector<mint> F = P[0], G = Q[0];
    F.resize(n + 1), G.resize(n + 1);
    return fps_div<mint>(F, G);
}
} // namespace mitsuha
#endif // AJAY_COEF_OF_FPS_POWS