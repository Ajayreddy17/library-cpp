#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_set_power_series"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/setfunc/sps_exp.hpp"

using mint = modint998;
void solve() {
    LL(N);
    vector<mint> s(1 << N); read(s);
    s = sps_exp<mint, 20>(s);
    print(s);
}

signed main() {
    solve();
    return 0;
}
