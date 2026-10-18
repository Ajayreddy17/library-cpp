#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"

#include <library/template.hpp>

#include <library/convex/dynamic_lichao.hpp>

struct F {
    using value_type = long long;  // operator() の戻り値
    int a;
    long long b;
    long long operator()(long long x) { return a * x + b; }
};

void solve() {
    LL(N, Q);
    Dynamic_LiChao_Tree<F, 0, 1> X(-infty<int>, infty<int>);
    using np = decltype(X)::np;
    np root = X.new_root();
    For(N) {
        LL(l, r, a, b);
        root = X.chmin_segment(root, l, r, {int(a), b});
    }
    vector<int> points;
    For(Q) {
        INT(t);
        if (t == 0) {
            LL(l, r, a, b);
            root = X.chmin_segment(root, l, r, {int(a), b});
        }
        if (t == 1) {
            INT(x);
            auto [fval, fid] = X.query(root, x);
            if (fid == -1) print("INFINITY");
            if (fid != -1) print(fval);
        }
    }
}

signed main() {
    solve();
    return 0;
}
