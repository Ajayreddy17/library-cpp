#define PROBLEM "https://judge.yosupo.jp/problem/area_of_union_of_rectangles"
#include <library/template.hpp>

#include <library/datastructure/rectangle_union.hpp>

void solve() {
    LL(N);
    Rectangle_Union<int> X;
    For(N) {
        LL(a, b, c, d);
        X.add_rect(a, c, b, d);
    }
    print(X.calc<long long>());
}

signed main() {
    cout << fixed << setprecision(15);

    solve();

    return 0;
}
