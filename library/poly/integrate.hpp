#ifndef AJAY_INTEGRATE
#define AJAY_INTEGRATE

namespace mitsuha{
template <typename mint>
vector<mint> integrate(const vector<mint>& f) {
    vector<mint> g((int) f.size() + 1);
    for(int i = 1; i < (int) g.size(); i++) g[i] = f[i - 1] * inv<mint>(i);
    return g;
}

// Indefinite integral: integrate(f)
// Definite integral: integrate(f, L, R)
template <typename mint>
mint integrate(const vector<mint>& f, mint L, mint R) {
    mint I = 0;
    mint pow_L = 1, pow_R = 1;
    for(int i = 0; i < len(f); i++) {
        pow_L *= L, pow_R *= R;
        I += inv<mint>(i + 1) * f[i] * (pow_R - pow_L);
    }
    return I;
}
} // namespace mitsuha
#endif // AJAY_INTEGRATE