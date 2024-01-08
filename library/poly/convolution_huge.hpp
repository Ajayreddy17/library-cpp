#ifndef AJAY_CONVOLUTION_HUGE
#define AJAY_CONVOLUTION_HUGE

#include "library/poly/convolution.hpp"

namespace mitsuha{
template <typename mint>
vector<mint> convolution_huge(const vector<mint>& A, const vector<mint>& B) {
    int N = len(A), M = len(B);
    if (N + M - 1 <= (1 << 22)) return convolution(A, B);
    long long L = 22;
    vector<vector<mint>> C(4, vector<mint>(2 << L));
    vector<vector<mint>> D(4, vector<mint>(2 << L));
    int mask = (1 << L) - 1;
    for(int i = 0; i < N; i++) C[i >> L][i & mask] = A[i];
    for(int i = 0; i < M; i++) D[i >> L][i & mask] = B[i];
    for(int i = 0; i < 4; i++) ntt(C[i], false);
    for(int i = 0; i < 4; i++) ntt(D[i], false);

    vector<mint> ANS(8 << L);

    for(int i = 0; i < 7; i++) {
        vector<mint> E(2 << L);
        for(int c = 0; c < 4; c++) for(int d = 0; d < 4; d++) if (c + d == i) {
            for(int k = 0; k < 2 << L; k++) E[k] += C[c][k] * D[d][k];
        }
        ntt(E, true);
        for(int k = 0; k < 2 << L; k++) ANS[(i << L) + k] += E[k];
    }
    ANS.resize(N + M - 1);
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_CONVOLUTION_HUGE