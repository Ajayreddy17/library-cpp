#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"
#include <library/template.hpp>
#include <library/geo/base.hpp>
#include <library/geo/angle_sort.hpp>

void solve() {
    LL(N);
    VT(pair<long long, long long>, P, N);
    auto I = angle_sort(P);
    P = rearrange(P, I);
    For(i, N) print(P[i]);
}

signed main() {
    solve();

    return 0;
}
