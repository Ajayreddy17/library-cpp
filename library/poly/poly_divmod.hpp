#ifndef AJAY_POLY_DIVMOD
#define AJAY_POLY_DIVMOD

#include "library/poly/fps_inv.hpp"

namespace mitsuha{
template <typename mint>
pair<vector<mint>, vector<mint>> poly_divmod(vector<mint> f, vector<mint> g) {
    assert(g.back() != 0);
    if (len(f) < len(g)) { return {{}, f}; }
    auto rf = f, rg = g;
    reverse(rf.begin(), rf.end()), reverse(rg.begin(), rg.end());
    long long deg = len(rf) - len(rg) + 1;
    rf.resize(deg), rg.resize(deg);
    rg = fps_inv(rg);
    auto q = convolution(rf, rg);
    q.resize(deg);
    reverse(q.begin(), q.end());
    auto h = convolution(q, g);
    For(i, len(f)) f[i] -= h[i];
    while (len(f) > 0 && f.back() == 0) f.pop_back();
    return {q, f};
}
} // namespace mitsuha
#endif // AJAY_POLY_DIVMOD