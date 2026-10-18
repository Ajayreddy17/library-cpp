#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"
#include <library/template.hpp>

#include <library/mod/floor_sum_of_linear.hpp>

void solve() {
    U32(N, mod, a, b);
    print(floor_sum_of_linear<uint64_t, uint64_t>(0, N, a, b, mod));
}

signed main() {
    LL(T);
    For(T) solve();

    return 0;
}
