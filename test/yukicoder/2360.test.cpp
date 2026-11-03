#define PROBLEM "https://yukicoder.me/problems/no/2360"
#include "library/template.hpp"
#include "library/graph/tree/rerooting_dp.hpp"
#include "library/mod/modint.hpp"

using mint = modint998;

void solve() {
    LL(N);
    vector<string> A(N); read(A);
    Graph<int, 0> G(N);
    G.read_tree();
    Tree<decltype(G)> tree(G);

    vector<mint> POW = {1};
    For(100) POW.emplace_back(POW.back() * mint(10));

    using Data = pair<mint, mint>;  // cnt, sum
    Data id = {mint(0), mint(0)};
    auto fee = [&](Data x, Data y) -> Data { return {x.first + y.first, x.second + y.second}; };
    auto fev = [&](Data x, int v) -> Data {
        mint val = stoi(A[v]);
        mint pow = POW[len(A[v])];
        return {x.first + mint(1), x.second * pow + val * (x.first + mint(1))};
    };
    // e は v に入る有向辺
    auto fve = [&](Data x, int r, int nxt_r) -> Data { return x; };
    Rerooting_DP<decltype(tree), Data> dp(tree, fee, fev, fve, id);

    vector<mint> ANS(N);
    For(v, N) ANS[v] = dp[v].second;
    // print(ANS);
    print(accumulate(ANS.begin(), ANS.end(), (mint)0));
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
