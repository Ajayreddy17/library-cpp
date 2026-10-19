#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_frequency"
#include <library/template.hpp>
#include <library/datastructure/fenwick_tree/fenwicktree_01.hpp>
#include <library/datastructure/hashmap.hpp>

void solve() {
    INT(N, Q);
    HashMap<int> MP(N + Q);
    vector<vector<pair<int, int>>> dat(N + Q);
    int p = 0;
    auto get = [&](int x) -> int {
        int k = MP.get(x, -1);
        return (k == -1 ? MP[x] = p++ : k);
    };

    vector<int> A(N);

    For(i, N) {
        INT(x);
        A[i] = get(x);
        dat[A[i]].emplace_back(0, i);
    }

    vector<pair<int, int>> LR;

    For(Q) {
        INT(t);
        if (t == 0) {
            INT(i, v);
            v = get(v);
            dat[A[i]].emplace_back(1, i);
            A[i] = v;
            dat[A[i]].emplace_back(0, i);
        }
        if (t == 1) {
            INT(L, R, x);
            x = get(x);
            dat[x].emplace_back(2, len(LR));
            LR.emplace_back(L, R);
        }
    }
    For(i, N) { dat[A[i]].emplace_back(1, i); }
    Q = len(LR);
    vector<int> ANS(Q);

    FenwickTree_01 bit(N);
    For(x, p) {
        for (auto& [t, i]: dat[x]) {
            if (t == 0) bit.add(i);
            elif (t == 1) bit.remove(i);
            else {
                auto [L, R] = LR[i];
                ANS[i] = bit.sum(L, R);
            }
        }
    }
    for (auto& x: ANS) print(x);
}

signed main() {
    solve();
    return 0;
}
