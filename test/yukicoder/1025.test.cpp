#define PROBLEM "https://yukicoder.me/problems/no/1025"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/mod/primitive_root.hpp"

using mint = modint107;

void solve() {
    LL(p, n, k, b);
    const int g = primitive_root(p);
    vector<int> EXP(p - 1);
    vector<int> LOG(p);
    {
        EXP[0] = 1;
        For(i, p - 2) EXP[i + 1] = EXP[i] * (long long)(g) % p;
        For(i, p - 1) LOG[EXP[i]] = i;
    }
    k = gcd(k, p - 1);
    vector<int> root(p);
    For(x, 1, p) {
        int e = LOG[x];
        e %= k;
        root[x] = EXP[e];
    }

    vector<int> S;
    for (int i = 0; i < p - 1; i += k) S.emplace_back(EXP[i]);
    vector<int> B;
    For(v, p) if (root[v] == v) B.emplace_back(v);

    vector<mint> dp(p);
    dp[0] = mint(1);
    For(n) {
        LL(a);
        vector<mint> newdp = dp;
        for (auto&& s : S) {
            int t = s * a % p;
            for (auto&& x : B) {
                int to = x + t;
                if (to >= p) to -= p;
                newdp[root[to]] += dp[x] * mint(k);
            }
        }
        swap(dp, newdp);
    }
    mint ANS = dp[root[b]];
    if (b != 0) ANS /= mint(len(S));
    print(ANS);
}

signed main() {
    solve();

    return 0;
}
