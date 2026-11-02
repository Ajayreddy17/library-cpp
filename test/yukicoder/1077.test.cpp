#define PROBLEM "https://yukicoder.me/problems/no/1077"
#include "library/template.hpp"
#include "library/convex/slope_trick/slope_trick_0.hpp"

void solve() {
    LL(N);
    Slope_Trick_0 X;
    For(N) {
        LL(x);
        X.add_abs(x);
        X.clear_right();
    }

    auto [lx, rx, min_f] = X.get_min();
    print(min_f);
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
