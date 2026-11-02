#define PROBLEM "https://yukicoder.me/problems/no/2525"
#include "library/template.hpp"
#include "library/bigint/base.hpp"

void solve() {
    STR(AA, BB);
    BigInteger A(AA), B(BB);
    bool ok = A.divmod(2).second == B.divmod(2).second;
    print(ok ? "Possible" : "Impossible");
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
