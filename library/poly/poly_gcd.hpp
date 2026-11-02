#ifndef AJAY_POLY_GCD
#define AJAY_POLY_GCD

#include "library/poly/poly_divmod.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/poly_gcd.hpp

// https://people.eecs.berkeley.edu/~fateman/282/readings/yap-2.pdf
namespace half_gcd {
template <typename T>
using arr = array<vector<T>, 2>;

template <typename T>
using mat = array<vector<T>, 4>;

template <typename T>
void shrink(vector<T>& a) {
    while (len(a) && a.back() == 0) a.pop_back();
}

template <typename T>
vector<T> operator+(const vector<T>& a, const vector<T>& b) {
    vector<T> c(max(len(a), len(b)));
    For(i, len(a)) c[i] += a[i];
    For(i, len(b)) c[i] += b[i];
    shrink(c);
    return c;
}

template <typename T>
vector<T> operator-(const vector<T>& a, const vector<T>& b) {
    vector<T> c(max(len(a), len(b)));
    For(i, len(a)) c[i] += a[i];
    For(i, len(b)) c[i] -= b[i];
    shrink(c);
    return c;
}

template <typename T>
vector<T> operator*(const vector<T>& a, const vector<T>& b) {
    return convolution(a, b);
}

template <typename T>
mat<T> operator*(const mat<T>& A, const mat<T>& B) {
    return {A[0] * B[0] + A[1] * B[2], A[0] * B[1] + A[1] * B[3],
                    A[2] * B[0] + A[3] * B[2], A[2] * B[1] + A[3] * B[3]};
}

template <typename T>
mat<T> step(const vector<T> q) {
    mat<T> Q;
    Q[1] = {1}, Q[2] = {1};
    Q[3] = Q[3] - q;
    return Q;
}

template <typename T>
arr<T> operator*(const mat<T>& A, const arr<T>& b) {
    return {A[0] * b[0] + A[1] * b[1], A[2] * b[0] + A[3] * b[1]};
}

template <typename T>
mat<T> hgcd(arr<T> a) {
    assert(len(a[0]) > len(a[1]) && len(a[1]) > 0);
    int m = len(a[0]) / 2;
    if (len(a[1]) <= m) {
        mat<T> M;
        M[0] = {1}, M[3] = {1};
        return M;
    }
    auto R = hgcd(arr<T>({vector<T>(a[0].begin() + m, a[0].end()),
                                                vector<T>(a[1].begin() + m, a[1].end())}));
    a = R * a;
    if (len(a[1]) <= m) return R;
    mat<T> Q = step(poly_divmod(a[0], a[1]).first);
    R = Q * R, a = Q * a;
    if (len(a[1]) <= m) return R;
    int k = 2 * m + 1 - len(a[0]);
    auto H = hgcd(arr<T>({vector<T>(a[0].begin() + k, a[0].end()),
                                                vector<T>(a[1].begin() + k, a[1].end())}));
    return H * R;
}

template <typename T>
mat<T> cgcd(arr<T> a) {
    assert(a[0].size() > a[1].size() && !a[1].empty());
    auto m0 = hgcd(a);
    a = m0 * a;
    if (a[1].empty()) return m0;
    mat<T> Q = step(poly_divmod(a[0], a[1]).first);
    m0 = Q * m0, a = Q * a;
    if (a[1].empty()) return m0;
    return cgcd(a) * m0;
}

// (gcd, fi, gi) with gcd == f * fi + g * gi
template <typename T>
tuple<vector<T>, vector<T>, vector<T>> poly_ext_gcd(const vector<T>& f, const vector<T>& g) {
    mat<T> Q = step(poly_divmod(f, g).first);
    auto m = Q;
    auto ap = Q * arr<T>{f, g};
    if (!ap[1].empty()) m = cgcd(ap) * m;
    return {f * m[0] + g * m[1], m[0], m[1]};
}

template <typename T>
vector<T> poly_gcd(vector<T> f, vector<T> g) {
    while (len(f) && f.back() == T(0)) POP(f);
    while (len(g) && g.back() == T(0)) POP(g);
    if (f.empty()) return g;
    if (g.empty()) return f;
    auto F = get<0>(poly_ext_gcd(f, g));
    T c = T(1) / F.back();
    for (auto& f : F) f *= c;
    return F;
}
}  // namespace half_gcd
using half_gcd::poly_ext_gcd;
using half_gcd::poly_gcd;
} // namespace mitsuha
#endif // AJAY_POLY_GCD
