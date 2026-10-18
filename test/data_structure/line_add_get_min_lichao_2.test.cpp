#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include <library/template.hpp>
#include <library/convex/lichao.hpp>

struct F {
    using value_type = long long;
    int a;
    long long b;
    long long operator()(long long x) { return a * x + b; }
};

void solve() {
    LL(N, Q);
    using T = tuple<int, int, long long>;
    vector<T> query;
    query.reserve(N + Q);
    For(N) {
        LL(a, b);
        query.emplace_back(0, a, b);
    }
    vector<long long> points;
    For(Q) {
        INT(t);
        if (t == 0) {
            LL(a, b);
            query.emplace_back(0, a, b);
        }
        if (t == 1) {
            INT(x);
            query.emplace_back(1, x, 0);
            points.emplace_back(x);
        }
    }
    LiChao_Tree<F, 1, 1> X(points, {0, infty<long long>});
    for (auto&& [t, a, b]: query) {
        if (t == 0) X.chmin_line({a, b});
        if (t == 1) print(X.query(a).first);
    }
}

signed main() {
    solve();
    return 0;
}
