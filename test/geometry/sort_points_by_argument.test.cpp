#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"
#include <library/template.hpp>
#include <library/geo/angle_sort.hpp>

void solve() {
    LL(N);
    vector<Point<long long>> P(N);
    For(i, N) read(P[i].x), read(P[i].y);
    auto I = angle_sort<long long>(P);
    P = rearrange(P, I);
    For(i, N) print(P[i].x, P[i].y);
}

signed main() {
    solve();

    return 0;
}
