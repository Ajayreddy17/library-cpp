#ifndef AJAY_FPS_DIV
#define AJAY_FPS_DIV

#include "library/poly/count_terms.hpp"
#include "library/poly/fps_inv.hpp"

namespace mitsuha{
// f/g. Output with length f.
template <typename mint, bool SPARSE = false>
vector<mint> fps_div(vector<mint> f, vector<mint> g) {
    if (SPARSE || count_terms(g) < 200) return fps_div_sparse(f, g);
    int n = len(f);
    g.resize(n);
    g = fps_inv<mint>(g);
    f = convolution(f, g);
    f.resize(n);
    return f;
}

// f/g where g is sparse
template <typename mint>
vector<mint> fps_div_sparse(vector<mint> f, vector<mint>& g) {
    if (g[0] != mint(1)) {
        mint cf = g[0].inverse();
        for (auto&& x: f) x *= cf;
        for (auto&& x: g) x *= cf;
    }

    vector<pair<int, mint>> dat;
    for(int i = 1; i < len(g); i++) if (g[i] != mint(0)) dat.emplace_back(i, -g[i]);
    for(int i = 0; i < len(f); i++) {
        for (auto&& [j, x]: dat) {
            if (i >= j) f[i] += x * f[i - j];
        }
    }
    return f;
}
} // namespace mitsuha
#endif // AJAY_FPS_DIV