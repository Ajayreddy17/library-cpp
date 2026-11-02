#define PROBLEM "https://yukicoder.me/problems/no/502"
#include "library/template.hpp"
#include "library/mod/factorial107.hpp"

void solve() {
    LL(N);
    print(factorial107(N));
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    long long T = 1;
    // LL(T);
    For(_, T) solve();

    return 0;
}
