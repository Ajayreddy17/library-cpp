#ifndef AJAY_DET
#define AJAY_DET

#include "library/mod/barrett.hpp"

namespace mitsuha{
int det_mod(vector<vector<int>> A, int mod) {
    Barrett bt(mod);
    const int n = len(A);
    long long det = 1;
    For(i, n) {
        For(j, i, n) {
            if (A[j][i] == 0) continue;
            if (i != j) { swap(A[i], A[j]), det = mod - det; }
            break;
        }
        For(j, i + 1, n) {
            while (A[i][i] != 0) {
                long long c = mod - A[j][i] / A[i][i];
                Frr(k, i, n) { A[j][k] = bt.modulo(A[j][k] + A[i][k] * c); }
                swap(A[i], A[j]), det = mod - det;
            }
            swap(A[i], A[j]), det = mod - det;
        }
    }
    For(i, n) det = bt.mul(det, A[i][i]);
    return det % mod;
}

template <typename mint>
mint det(vector<vector<mint>>& A) {
    const int n = len(A);
    vector<vector<int>> B(n, vector<int>(n));
    For(i, n) For(j, n) B[i][j] = A[i][j].val;
    return det_mod(B, mint::get_mod());
}
} // namespace mitsuha
#endif // AJAY_DET