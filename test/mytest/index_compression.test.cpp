#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <library/template.hpp>

#include <library/datastructure/index_compression.hpp>
#include <library/random/base.hpp>

template <bool SMALL>
void test_distinct() {
    vector<int> X = {3, 4, 7, 4, 3, 6, 6, 7, 3};
    Index_Compression<int, false, SMALL> IDX;
    vector<int> Y = IDX.build(X);
    assert(Y == vector<int>({0, 3, 7, 4, 1, 5, 6, 8, 2}));
    assert(IDX.val_to_idx(1) == 0 && IDX.val_to_idx(2) == 0 &&
                 IDX.val_to_idx(3) == 0);
    assert(IDX.val_to_idx(4) == 3 && IDX.val_to_idx(5) == 5 &&
                 IDX.val_to_idx(6) == 5);
    assert(IDX.val_to_idx(7) == 7 && IDX.val_to_idx(8) == 9 &&
                 IDX.val_to_idx(9) == 9);

    For(100) {
        For(N, 0, 300) {
            vector<int> X(N);
            For(i, N) X[i] = RNG(-100, 100);
            Index_Compression<int, false, SMALL> IDX;
            vector<int> Y = IDX.build(X);
            auto I = argsort(X);
            For(i, N) assert(Y[I[i]] == i);
            X = rearrange(X, I);
            For(x, -100, 100) assert(IDX.val_to_idx(x) == lower_bound(X, x));
        }
    }
}

template <bool SMALL>
void test_same() {
    vector<int> X = {3, 4, 7, 4, 3, 6, 6, 7, 3};
    Index_Compression<int, true, SMALL> IDX;
    vector<int> Y = IDX.build(X);
    assert(Y == vector<int>({0, 1, 3, 1, 0, 2, 2, 3, 0}));
    assert(IDX.val_to_idx(1) == 0 && IDX.val_to_idx(2) == 0 &&
                 IDX.val_to_idx(3) == 0);
    assert(IDX.val_to_idx(4) == 1 && IDX.val_to_idx(5) == 2 &&
                 IDX.val_to_idx(6) == 2);
    assert(IDX.val_to_idx(7) == 3 && IDX.val_to_idx(8) == 4 &&
                 IDX.val_to_idx(9) == 4);

    For(100) {
        For(N, 0, 300) {
            vector<int> X(N);
            For(i, N) X[i] = RNG(-100, 100);
            Index_Compression<int, true, SMALL> IDX;
            vector<int> Y = IDX.build(X);
            vector<int> key = X;
            settify(key);
            For(i, N) assert(lower_bound(key, X[i]) == Y[i]);
            For(x, -100, 100) assert(IDX.val_to_idx(x) == lower_bound(key, x));
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test_distinct<false>();
    test_distinct<true>();
    test_same<false>();
    test_same<true>();
    solve();
}