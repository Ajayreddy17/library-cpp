#define PROBLEM "https://judge.yosupo.jp/problem/min_of_mod_of_linear"
#include "library/template.hpp"
#include "library/mod/range_freq_of_linear.hpp"
#include "library/util/binary_serch.hpp"

void solve() {
    LL(n, m, a, b);
    // largest k with no x in [0, n) such that (ax + b) mod m < k
    auto check = [&](long long k) -> bool {
        return range_freq_of_linear<__int128, long long>(0, n, a, b, m, 0, k) == 0;
    };
    print(binary_search<LastTrue>(0LL, m - 1, check));
}

signed main() {
    LL(T);
    For(T) solve();
    return 0;
}
