#define PROBLEM "https://yukicoder.me/problems/no/2897"
#include "library/template.hpp"
#include "library/geo/manhattan_nns.hpp"

void solve() {
    LL(N);
    vector<pair<long long, long long>> FRM(N); read(FRM);
    LL(M);
    vector<pair<long long, long long>> TO(M); read(TO);
    auto [D, J] = manhattan_nns(FRM, TO);
    print(*min_element(D.begin(), D.end()));
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
