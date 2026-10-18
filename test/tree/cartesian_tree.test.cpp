#define PROBLEM "https://judge.yosupo.jp/problem/cartesian_tree"
#include <library/template.hpp>
#include <library/datastructure/cartesian_tree.hpp>

void solve() {
    LL(N);
    VT(int, A, N);
    CartesianTree<int, 1> CT(A);
    auto& ANS = CT.par;
    For(i, N) if (ANS[i] == -1) ANS[i] = i;
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
