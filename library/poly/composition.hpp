#ifndef AJAY_COMPOSITION
#define AJAY_COMPOSITION

#include "library/poly/convolution.hpp"
#include "library/poly/poly_taylor_shift.hpp"
#include "library/poly/transposed_ntt.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/composition.hpp


template <typename mint>
vector<mint> composition_old(vector<mint>& Q, vector<mint>& P) {
    int n = len(P);
    assert(len(P) == len(Q));
    int k = 1;
    while (k * k < n) ++k;
    // compute powers of P
    vector<vector<mint>> pow1(k + 1, vector<mint>());
    pow1[0] = {1};
    pow1[1] = P;
    For(i, 2, k + 1) {
        pow1[i] = convolution(pow1[i - 1], pow1[1]);
        pow1[i].resize(n);
    }
    vector<vector<mint>> pow2(k + 1, vector<mint>());
    pow2[0] = {1};
    pow2[1] = pow1[k];
    For(i, 2, k + 1) {
        pow2[i] = convolution(pow2[i - 1], pow2[1]);
        pow2[i].resize(n);
    }
    vector<mint> ANS(n);
    For(i, k + 1) {
        vector<mint> f(n);
        For(j, k) {
            if (k * i + j < len(Q)) {
                mint coef = Q[k * i + j];
                For(d, len(pow1[j])) f[d] += pow1[j][d] * coef;
            }
        }
        f = convolution(f, pow2[i]);
        f.resize(n);
        For(d, n) ANS[d] += f[d];
    }
    return ANS;
}

// f(g(x)), O(Nlog^2N)
template <typename mint>
vector<mint> composition_0_ntt(vector<mint> f, vector<mint> g) {
    assert(len(f) == len(g));
    if (f.empty()) return {};

    int n0 = len(f);
    int n = 1;
    while (n < len(f)) n *= 2;
    f.resize(n), g.resize(n);

    vector<mint> W(n);
    {
        // bit reverse order
        vector<int> btr(n);
        int log = msb(n);
        For(i, n) { btr[i] = (btr[i >> 1] >> 1) + ((i & 1) << (log - 1)); }
        int t = mint::ntt_info().first;
        mint r = mint::ntt_info().second;
        mint dw = r.inverse().pow((1 << t) / (2 * n));
        mint w = 1;
        for (auto& i : btr) {
            W[i] = w, w *= dw;
        }
    }

    auto rec = [&](auto& rec, int n, int k, vector<mint>& Q) -> vector<mint> {
        if (n == 1) {
            reverse(f.begin(), f.end());
            transposed_ntt(f, 1);
            mint c = mint(1) / mint(k);
            for (auto& x : f) x *= c;
            vector<mint> p(4 * k);
            For(i, k) p[2 * i] = f[i];
            return p;
        }
        auto doubling_y = [&](vector<mint>& A, int l, int r, bool t) -> void {
            mint z = W[k / 2].inverse();
            vector<mint> f(k);
            if (!t) {
                For(i, l, r) {
                    For(j, k) f[j] = A[2 * n * j + i];
                    ntt(f, 1);
                    mint r = 1;
                    For(j, 1, k) r *= z, f[j] *= r;
                    ntt(f, 0);
                    For(j, k) A[2 * n * (k + j) + i] = f[j];
                }
            } else {
                For(i, l, r) {
                    For(j, k) f[j] = A[2 * n * (k + j) + i];
                    transposed_ntt(f, 0);
                    mint r = 1;
                    For(j, 1, k) r *= z, f[j] *= r;
                    transposed_ntt(f, 1);
                    For(j, k) A[2 * n * j + i] += f[j];
                }
            }
        };

        auto FFT_x = [&](vector<mint>& A, int l, int r, bool t) -> void {
            vector<mint> f(2 * n);
            if (!t) {
                For(j, l, r) {
                    move(A.begin() + 2 * n * j, A.begin() + 2 * n * (j + 1), f.begin());
                    ntt(f, 0);
                    move(f.begin(), f.end(), A.begin() + 2 * n * j);
                }
            } else {
                For(j, l, r) {
                    move(A.begin() + 2 * n * j, A.begin() + 2 * n * (j + 1), f.begin());
                    transposed_ntt(f, 0);
                    move(f.begin(), f.end(), A.begin() + 2 * n * j);
                }
            }
        };

        if (n <= k) doubling_y(Q, 1, n, 0), FFT_x(Q, 0, 2 * k, 0);
        if (n > k) FFT_x(Q, 0, k, 0), doubling_y(Q, 0, 2 * n, 0);

        For(i, 2 * n * k) Q[i] += 1;
        For(i, 2 * n * k, 4 * n * k) Q[i] -= 1;

        vector<mint> nxt_Q(4 * n * k);
        vector<mint> F(2 * n), G(2 * n), f(n), g(n);
        For(j, 2 * k) {
            move(Q.begin() + 2 * n * j, Q.begin() + 2 * n * j + 2 * n, G.begin());
            For(i, n) { g[i] = G[2 * i] * G[2 * i + 1]; }
            ntt(g, 1);
            move(g.begin(), g.begin() + n / 2, nxt_Q.begin() + n * j);
        }
        For(j, 4 * k) nxt_Q[n * j] = 0;

        vector<mint> p = rec(rec, n / 2, k * 2, nxt_Q);
        Frr(j, 2 * k) {
            move(p.begin() + n * j, p.begin() + n * j + n / 2, f.begin());
            move(Q.begin() + 2 * n * j, Q.begin() + 2 * n * j + 2 * n, G.begin());
            fill(f.begin() + n / 2, f.end(), mint(0));
            transposed_ntt(f, 1);
            For(i, n) {
                f[i] *= W[i];
                F[2 * i] = G[2 * i + 1] * f[i], F[2 * i + 1] = -G[2 * i] * f[i];
            }
            move(F.begin(), F.end(), p.begin() + 2 * n * j);
        }
        if (n <= k) FFT_x(p, 0, 2 * k, 1), doubling_y(p, 0, n, 1);
        if (n > k) doubling_y(p, 0, 2 * n, 1), FFT_x(p, 0, k, 1);
        return p;
    };

    vector<mint> Q(4 * n);
    For(i, n) Q[i] = -g[i];

    vector<mint> p = rec(rec, n, 1, Q);
    p.resize(n);
    reverse(p.begin(), p.end());
    p.resize(n0);
    return p;
}

template <typename mint>
vector<mint> composition_0_garner(vector<mint> f, vector<mint> g) {
    constexpr uint32_t ps[] = {167772161, 469762049, 754974721};
    using mint0 = modint<ps[0]>;
    using mint1 = modint<ps[1]>;
    using mint2 = modint<ps[2]>;

    auto rec = [&](auto& rec, int n, int k, vector<mint> Q) -> vector<mint> {
        if (n == 1) {
            vector<mint> p(2 * k);
            reverse(f.begin(), f.end());
            For(i, k) p[2 * i] = f[i];
            return p;
        }
        vector<mint0> Q0(4 * n * k), R0(4 * n * k), p0(4 * n * k);
        vector<mint1> Q1(4 * n * k), R1(4 * n * k), p1(4 * n * k);
        vector<mint2> Q2(4 * n * k), R2(4 * n * k), p2(4 * n * k);
        For(i, 2 * n * k) {
            Q0[i] = Q[i].val, R0[i] = (i % 2 == 0 ? Q[i].val : (-Q[i]).val);
            Q1[i] = Q[i].val, R1[i] = (i % 2 == 0 ? Q[i].val : (-Q[i]).val);
            Q2[i] = Q[i].val, R2[i] = (i % 2 == 0 ? Q[i].val : (-Q[i]).val);
        }
        ntt(Q0, 0), ntt(Q1, 0), ntt(Q2, 0), ntt(R0, 0), ntt(R1, 0), ntt(R2, 0);
        For(i, 4 * n * k) Q0[i] *= R0[i], Q1[i] *= R1[i], Q2[i] *= R2[i];
        ntt(Q0, 1), ntt(Q1, 1), ntt(Q2, 1);
        vector<mint> QQ(4 * n * k);
        For(i, 4 * n * k) {
            QQ[i] = CRT3<mint, ps[0], ps[1], ps[2]>(Q0[i].val, Q1[i].val, Q2[i].val);
        }
        for (int i = 0; i < 2 * n * k; i += 2) QQ[2 * n * k + i] += Q[i] + Q[i];
        vector<mint> nxt_Q(2 * n * k);
        For(j, 2 * k) For(i, n / 2) {
            nxt_Q[n * j + i] = QQ[(2 * n) * j + (2 * i + 0)];
        }

        vector<mint> nxt_p = rec(rec, n / 2, k * 2, nxt_Q);
        vector<mint> pq(4 * n * k);
        For(j, 2 * k) For(i, n / 2) {
            pq[(2 * n) * j + (2 * i + 1)] += nxt_p[n * j + i];
        }

        vector<mint> p(2 * n * k);
        For(i, 2 * n * k) { p[i] += pq[2 * n * k + i]; }
        For(i, 4 * n * k) {
            p0[i] += pq[i].val, p1[i] += pq[i].val, p2[i] += pq[i].val;
        }
        transposed_ntt(p0, 1), transposed_ntt(p1, 1), transposed_ntt(p2, 1);
        For(i, 4 * n * k) p0[i] *= R0[i], p1[i] *= R1[i], p2[i] *= R2[i];
        transposed_ntt(p0, 0), transposed_ntt(p1, 0), transposed_ntt(p2, 0);
        For(i, 2 * n * k) {
            p[i] += CRT3<mint, ps[0], ps[1], ps[2]>(p0[i].val, p1[i].val, p2[i].val);
        }
        return p;
    };
    assert(len(f) == len(g));
    int n = 1;
    while (n < len(f)) n *= 2;
    int out_len = len(f);
    f.resize(n), g.resize(n);
    int k = 1;
    vector<mint> Q(2 * n);
    For(i, n) Q[i] = -g[i];
    vector<mint> p = rec(rec, n, k, Q);

    vector<mint> output(n);
    For(i, n) output[i] = p[i];
    reverse(output.begin(), output.end());
    output.resize(out_len);
    return output;
}

template <typename mint>
vector<mint> composition(vector<mint> f, vector<mint> g) {
    assert(len(f) == len(g));
    if (f.empty()) return {};
    // reduce to [x^0]g = 0
    if (g[0] != mint(0)) {
        f = poly_taylor_shift<mint>(f, g[0]);
        g[0] = 0;
    }
    if (mint::can_ntt()) {
        return composition_0_ntt(f, g);
    }
    return composition_0_garner(f, g);
}
} // namespace mitsuha
#endif // AJAY_COMPOSITION
