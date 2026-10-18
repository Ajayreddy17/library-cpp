#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"
#include <library/template.hpp>

#include <library/monoid/monoid_affine.hpp>
#include <library/datastructure/sliding_window_aggregation.hpp>
#include <library/mod/modint.hpp>

using mint = modint998;

void solve() {
    LL(Q);
    using Mono = Monoid_Affine<mint>;
    using F = Mono::value_type;

    Sliding_Window_Aggregation<Mono> swag;

    For(_, Q) {
        LL(t);
        if (t == 0) {
            LL(a, b);
            swag.push(F({a, b}));
        }
        elif (t == 1) { swag.pop(); }
        elif (t == 2) {
            LL(x);
            F f = swag.prod();
            print(Mono::eval(f, x));
        }
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
