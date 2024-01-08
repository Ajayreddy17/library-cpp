#ifndef AJAY_DIFFERENTIATE
#define AJAY_DIFFERENTIATE

namespace mitsuha{
template <typename mint>
vector<mint> differentiate(const vector<mint>& f) {
    if ((int) f.size() <= 1) return {};
    vector<mint> g((int) f.size() - 1);
    for(int i = 0; i < (int) g.size(); i++) g[i] = f[i + 1] * mint(i + 1);
    return g;
}
} // namespace mitsuha
#endif // AJAY_DIFFERENTIATE