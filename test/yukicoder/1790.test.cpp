#define PROBLEM "https://yukicoder.me/problems/no/1790"
#include "library/template.hpp"
#include "library/monoid/monoid_xor.hpp"
#include "library/monoid/monoid_max.hpp"
#include "library/graph/datastructure/lazy_tree_monoid.hpp"

struct ActedMonoid {
    using Monoid_X = Monoid_Xor<long long>;
    using Monoid_A = Monoid_Max<int>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        if (a == Monoid_A::unit()) return x;
        return 0;
    }
};

void solve() {
    LL(N);
    Graph<int, 0> G(N);
    vector<long long> dat;
    For(N - 1) {
        LL(a, b, x);
        --a, --b;
        G.add(a, b);
        dat.emplace_back(x);
    }
    G.build();
    Tree<decltype(G)> tree(G);

    Lazy_Tree_Monoid<decltype(tree), ActedMonoid, 1> X(tree, dat);
    LL(Q);
    For(Q) {
        LL(t, x);
        --x;
        if (t == 2) { print(X.prod_subtree(x)); }
        if (t == 1) {
            X.apply_subtree(x, 0);
            if (x) X.apply_path(x, tree.parent[x], 0);
        }
    }
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
