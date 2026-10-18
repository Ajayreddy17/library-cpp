#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include <library/template.hpp>

#include <library/number/factor.hpp>

void solve() {
    LL(x);
    auto pf = factor(x);
    vector<long long> ANS;
    for (auto&& [p, e]: pf) { For(_, e) ANS.emplace_back(p); }
    if (len(ANS))
        print(len(ANS), ANS);
    else
        print(0);
}

signed main() {
    INT(T);
    For(T) solve();

    return 0;
}
