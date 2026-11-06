#ifndef AJAY_MULTIPLICATIVE_CONVOLUTION_MOD_2N
#define AJAY_MULTIPLICATIVE_CONVOLUTION_MOD_2N

#include "library/poly/convolution.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/multiplicative_convolution_mod_2n.hpp
// C[k] = sum_{ij = k mod 2^N} A[i] B[j], len(A) = len(B) = 2^N
// x = 2^n * (+-5^j): split by n, then 2D cyclic convolution over (sign, j)
template <typename mint>
vector<mint> multiplicative_convolution_mod_2n(vector<mint> &A, vector<mint> &B) {
    int N = 0;
    while ((1 << N) < len(A)) ++N;
    assert((1 << N) == len(A) && (1 << N) == len(B));
    const int mask = (1 << N) - 1;

    vector<vector<vector<mint>>> AA(N + 1);
    vector<vector<vector<mint>>> BB(N + 1);
    vector<vector<vector<mint>>> CC(N + 1);

    auto shape = [&](int n) -> pair<int, int> {
        int H = (N - n >= 2 ? 2 : 1);
        int W = 1 << max(N - n - 2, 0);
        return {H, W};
    };

    For(n, N + 1) {
        // x divisible by 2 exactly n times
        auto [H, W] = shape(n);
        AA[n].assign(H, vector<mint>(W));
        BB[n].assign(H, vector<mint>(W));
        CC[n].assign(H, vector<mint>(W));
        int x = (1 << n) & mask;
        auto &a = AA[n], &b = BB[n];
        For(j, W) {
            a[0][j] = A[x];
            b[0][j] = B[x];
            if (H == 2) {
                a[1][j] = A[(1 << N) - x];
                b[1][j] = B[(1 << N) - x];
            }
            x = (5 * x) & mask;
        }
    }
    // fft along each axis for fixed n: O(N 2^N) in total
    For(n, N + 1) {
        auto &a = AA[n], &b = BB[n];
        auto [H, W] = shape(n);
        For(i, H) {
            ntt(a[i], false);
            ntt(b[i], false);
        }
        if (H == 2) {
            For(j, W) {
                tie(a[0][j], a[1][j]) = make_pair(a[0][j] + a[1][j], a[0][j] - a[1][j]);
                tie(b[0][j], b[1][j]) = make_pair(b[0][j] + b[1][j], b[0][j] - b[1][j]);
            }
        }
    }
    For(n1, N + 1) For(n2, N + 1) {
        // pointwise products of the needed length, added where they belong: O(2^N) in total
        int n3 = min(N, int(n1 + n2));
        auto [H, W] = shape(n3);
        For(i, H) For(j, W) CC[n3][i][j] += AA[n1][i][j] * BB[n2][i][j];
    }

    For(n, N + 1) {
        // inverse fft
        auto &c = CC[n];
        auto [H, W] = shape(n);
        For(i, H) ntt(c[i], true);
        if (H == 2) {
            For(j, W) { tie(c[0][j], c[1][j]) = make_pair(c[0][j] + c[1][j], c[0][j] - c[1][j]); }
        }
        mint coef = mint(1) / mint(H);
        For(i, H) For(j, W) c[i][j] *= coef;
    }

    vector<mint> C(1 << N);
    For(n, N + 1) {
        auto [H, W] = shape(n);
        int x = (1 << n) & mask;
        auto &c = CC[n];
        For(j, W) {
            C[x] = c[0][j];
            if (H == 2) { C[(1 << N) - x] = c[1][j]; }
            x = (5 * x) & mask;
        }
    }
    return C;
}
} // namespace mitsuha
#endif // AJAY_MULTIPLICATIVE_CONVOLUTION_MOD_2N
