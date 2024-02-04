#ifndef AJAY_ROLLINGHASH
#define AJAY_ROLLINGHASH

#include "library/random/base.hpp"
#include "library/mod/modint61.hpp"

namespace mitsuha{
struct RollingHash {
    using mint = modint61;
    static constexpr unsigned long long mod = mint::get_mod();
    const mint base;
    vector<mint> power;

    static inline mint generate_base() { return RNG(mod); }

    inline void expand(size_t sz) {
        if (power.size() < sz + 1) {
            int pre_sz = (int)power.size();
            power.resize(sz + 1);
            for(int i = pre_sz - 1; i < sz; ++i) power[i + 1] = power[i] * base;
        }
    }

    explicit RollingHash(mint base = generate_base()) : base(base), power{1} {}

    template <typename STRING>
    vector<mint> build(const STRING& s) const {
        int sz = s.size();
        vector<mint> hashed(sz + 1, mint(0));
        for (int i = 0; i < sz; i++) { hashed[i + 1] = hashed[i] * base + s[i]; }
        return hashed;
    }

    template <typename STRING>
    mint eval(STRING& s) {
        mint x = 0;
        for (auto& ch: s) x = base * x + ch;
        return x;
    }

    mint query(const vector<mint>& s, int l, int r) {
        expand(r - l);
        return (s[r] - s[l] * power[r - l]);
    }

    mint combine(mint h1, mint h2, int h2len) {
        expand(h2len);
        return h1 * power[h2len] + h2;
    }

    mint add_char(mint h, int x) { return h * base + mint(x); }

    int lcp(const vector<mint>& a, int l1, int r1, const vector<mint>& b, int l2, int r2) {
        int len = min(r1 - l1, r2 - l2);
        int low = 0, high = len + 1;
        while (high - low > 1) {
            int mid = (low + high) / 2;
            if (query(a, l1, l1 + mid) == query(b, l2, l2 + mid))
                low = mid;
            else
                high = mid;
        }
        return low;
    }
};
} // namespace mitsuha
#endif // AJAY_ROLLINGHASH
