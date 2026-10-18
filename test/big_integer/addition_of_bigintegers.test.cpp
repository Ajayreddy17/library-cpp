#define PROBLEM "https://judge.yosupo.jp/problem/addition_of_big_integers"
#include <library/template.hpp>

#include <library/bigint/base.hpp>

using B = BigInteger;

void solve() {
    auto get = [&]() -> B {
        STR(S);
        return B(S);
    };
    B ANS = get() + get();
    print(ANS.to_string());
}

signed main() {
    INT(T);
    For(T) solve();

    return 0;
}
