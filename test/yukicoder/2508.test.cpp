#define PROBLEM "https://yukicoder.me/problems/no/2508"
#include "library/template.hpp"
#include "library/bigint/base.hpp"

using B = BigInteger;
void solve() {
    STR(A, P, Q);
    B a(A), p(P), q(Q);
    B b = a * (p + q);
    B c = a * p * q;
    print(Yes[b * b > B(4) * a * c]);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
