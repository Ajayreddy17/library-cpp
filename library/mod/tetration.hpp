#ifndef AJAY_TETRATION
#define AJAY_TETRATION

#include "library/mod/barrett.hpp"
#include "library/number/euler_phi.hpp"

namespace mitsuha{
// a[0]^(a[1]^(a[2]^...))
int tetration(vector<long long> a, int mod) {
    for (auto&& x: a) assert(x > 0);
    
    vector<int> mod_chain = {mod};
    while (mod_chain.back() > 1) mod_chain.emplace_back(euler_phi(mod_chain.back()));
    while (len(a) > len(mod_chain)) a.pop_back();
    while (len(mod_chain) > len(a)) mod_chain.pop_back();

    auto pow = [&](long long x, int n, int mod) -> int {
        Barrett bt(mod);
        if (x >= mod) x = bt.modulo(x) + mod;
        long long v = 1;
        do {
            if (n & 1) {
                v *= x;
                if (v >= mod) v = bt.modulo(v) + mod;
            }
            x *= x;
            if (x >= mod) x = bt.modulo(x) + mod;
            n /= 2;
        } while (n);
        return v;
    };

    int v = 1;
    Frr(i, len(a)) v = pow(a[i], v, mod_chain[i]);
    return v % mod;
}
} // namespace mitsuha
#endif // AJAY_TETRATION