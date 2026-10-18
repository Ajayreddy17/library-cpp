#define PROBLEM "https://judge.yosupo.jp/problem/deque_operate_all_composite"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/monoid/monoid_affine.hpp>
#include <library/datastructure/sliding_window_aggregation.hpp>

using mint = modint998;

void solve() {
    LL(Q);
    using G = Monoid_Affine<mint>;
    Sliding_Window_Aggregation_Deque<G> X;
    For(Q) {
        LL(t);
        if (t == 0) {
            INT(a, b);
            X.push_front({a, b});
        }
        if (t == 1) {
            INT(a, b);
            X.push_back({a, b});
        }
        if (t == 2) {
            X.pop_front();
        }
        if (t == 3) {
            X.pop_back();
        }
        if (t == 4) {
            INT(x);
            auto f = X.prod();
            print(G::eval(f, x));
        }
    }
}

signed main() {
    solve();
    return 0;
}
