#define PROBLEM "https://yukicoder.me/problems/no/1339"
#include "library/template.hpp"
#include "library/number/discrete_log.hpp"
#include "library/mod/dynamic_modint.hpp"
#include "library/monoid/monoid_mul.hpp"

void solve() {
    LL(N);
    while (N % 2 == 0) N /= 2;
    while (N % 5 == 0) N /= 5;
    dmint::set_mod(N);
    auto H = [&](dmint x) -> long long { return x.val; };
    long long k = discrete_log_monoid<Monoid_Mul<dmint>>(dmint(10), dmint(1), H, 1, N + 10);
    print(k);
}

signed main() {
    LL(T);
    For(T) solve();

    return 0;
}
