#define PROBLEM "https://yukicoder.me/problems/no/1976#"
#include "library/template.hpp"
#include "library/graph/tree/rerooting_dp.hpp"

void solve() {
    LL(N);
    Graph<int, 0> G(N);
    G.read_tree();
    Tree<decltype(G)> tree(G);

    using Data = pair<int, int>;
    Data id = {0, 0};
    auto fee = [&](Data x, Data y) -> Data {
        return {max({x.first, y.first, x.second + y.second}), max(x.second, y.second)};
    };
    auto fev = [&](Data x, int v) -> Data { return x; };
    // e は v から出る有向辺
    auto fve = [&](Data x, int r, int nxt_r) -> Data { return {x.first, x.second + 1}; };
    Rerooting_DP<decltype(tree), Data> dp(tree, fee, fev, fve, id);

    int ANS = N;

    for (auto&& e : G.edges) {
        int a = e.frm, b = e.to;
        int xa = dp.get(a, b).first;
        int xb = dp.get(b, a).first;
        int ra = cld(xa, 2);
        int rb = cld(xb, 2);
        int x = max({xa, xb, 1 + ra + rb});
        chmin(ANS, x);
    }
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
