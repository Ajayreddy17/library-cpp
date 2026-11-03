#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/enumerate/product.hpp"
#include "library/graph/tree/prufer_code.hpp"

void test(int n) {
    /*
    F: sequence -> tree
    G: tree -> sequence
    - both are maps
    - GF(A)=A
    given these, and that the counts are equal, it is a bijection
    */
    enumerate_product(vector<int>(n - 1, n), [&](vector<int> A) -> void {
        if (n >= 2 && A.back() != n - 1) return;
        Graph<int, 0> G = from_prufer_code(A);
        UnionFind uf(n);
        for (auto& e : G.edges) assert(uf.merge(e.frm, e.to));
        assert(uf.n_comp == 1);
        vector<int> B = to_prufer_code(G);
        assert(A == B);
    });
}

void solve() {
    int x, y;
    cin >> x >> y;
    cout << x + y << "\n";
}

signed main() {
    For(n, 2, 8) test(n);
    solve();
    return 0;
}
