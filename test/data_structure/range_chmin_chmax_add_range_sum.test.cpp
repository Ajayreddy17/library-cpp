#define PROBLEM \
    "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"

#include <library/template.hpp>
#include <library/datastructure/segment_tree/beats_summinmax_chminchmax.hpp>

void solve() {
    LL(N, Q);
    VT(long long, A, N);
    Beats_SumMinMax_ChminChmax<long long> seg(A);
    For(_, Q) {
        LL(t, L, R);
        if (t == 0) {
            LL(x);
            seg.chmin(L, R, x);
        }
        elif (t == 1) {
            LL(x);
            seg.chmax(L, R, x);
        }
        elif (t == 2) {
            LL(x);
            seg.add(L, R, x);
        }
        elif (t == 3) { print(get<0>(seg.prod(L, R))); }
    }
}

signed main() {
    solve();
    return 0;
}
