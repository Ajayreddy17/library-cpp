#define PROBLEM "https://yukicoder.me/problems/no/2587"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/base.hpp"
#include "library/poly/poly_taylor_shift.hpp"
#include "library/poly/coef_of_rational_fps.hpp"
#include "library/graph/tree_walk_generating_function.hpp"

using mint = modint998;
using poly = vector<mint>;

void solve() {
    LL(N, M, S, T);
    --S, --T;
    Graph<int, 0> G(N);
    G.read_tree();

    auto [g, f] = tree_walk_generating_function<true, mint>(
            G, S, T, [&](int i, int j) -> mint { return 1; });

    mint ANS = coef_of_rational_fps<mint>(g, f, M);
    print(ANS);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
