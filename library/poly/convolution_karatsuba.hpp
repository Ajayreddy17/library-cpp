#ifndef AJAY_CONV_KARATSUBA
#define AJAY_CONV_KARATSUBA

namespace mitsuha{
// Can be done with any ring
template <typename T>
vector<T> convolution_karatsuba(const vector<T>& f, const vector<T>& g) {
    const int thresh = 30;
    if (min(len(f), len(g)) <= thresh) return convolution_naive(f, g);
    int n = max(len(f), len(g));
    int m = (n + 1) / 2;
    vector<T> f1, f2, g1, g2;
    if (len(f) < m) f1 = f;
    if (len(f) >= m) f1 = {f.begin(), f.begin() + m};
    if (len(f) >= m) f2 = {f.begin() + m, f.end()};
    if (len(g) < m) g1 = g;
    if (len(g) >= m) g1 = {g.begin(), g.begin() + m};
    if (len(g) >= m) g2 = {g.begin() + m, g.end()};
    vector<T> a = convolution_karatsuba(f1, g1);
    vector<T> b = convolution_karatsuba(f2, g2);
    for(int i = 0; i < (int) f2.size(); i++) f1[i] += f2[i];
    for(int i = 0; i < (int) g2.size(); i++) g1[i] += g2[i];
    vector<T> c = convolution_karatsuba(f1, g1);
    vector<T> F((int) f.size() + (int) g.size() - 1);
    assert(2 * m + len(b) <= len(F));
    for(int i = 0; i < (int) a.size(); i++) F[i] += a[i], c[i] -= a[i];
    for(int i = 0; i < (int) b.size(); i++) F[2 * m + i] += b[i], c[i] -= b[i];
    if (c.back() == T(0)) c.pop_back();
    for(int i = 0; i < (int) c.size(); i++) if (c[i] != T(0)) F[m + i] += c[i];
    return F;
}
} // namespace mitsuha
#endif // AJAY_CONV_KARATSUBA