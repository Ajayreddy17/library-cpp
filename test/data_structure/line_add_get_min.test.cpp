#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include <library/template.hpp>
#include <library/convex/cht.hpp>

void solve() {
    LL(N, Q);
    CHT_Min<long long> cht_min;
    CHT_Max<long long> cht_max;
    For(N) {
        LL(a, b);
        cht_min.add(a, b);
        cht_max.add(-a, -b);
    }
    For(Q) {
        LL(t);
        if (t == 0) {
            LL(a, b);
            cht_min.add(a, b);
            cht_max.add(-a, -b);
        } else {
            LL(x);
            long long y = cht_min.query(x);
            long long z = cht_max.query(x);
            assert(y + z == 0);
            print(y);
        }
    }
}

signed main() {
    solve();

    return 0;
}
