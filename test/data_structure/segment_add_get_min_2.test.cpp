#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"
#include <library/template.hpp>
#include <library/convex/lichao.hpp>

struct F {
    using value_type = long long; // operator() の戻り値
    int a;
    long long b;
    long long operator()(long long x) { return a * x + b; }
};

void solve() {
    LL(N, Q);
    using T = tuple<int, int, int, int, long long>;
    vector<T> query;
    query.reserve(N + Q);
    For(N) {
        LL(l, r, a, b);
        query.emplace_back(0, l, r, a, b);
    }
    vector<long long> points;
    For(Q) {
        INT(t);
        if (t == 0) {
            LL(l, r, a, b);
            query.emplace_back(0, l, r, a, b);
        }
        if (t == 1) {
            INT(x);
            query.emplace_back(1, x, 0, 0, 0);
            points.emplace_back(x);
        }
    }
    LiChao_Tree<F, 1, 1> X(points, {0, infty<long long>});
    for (auto&& [t, l, r, a, b]: query) {
        if (t == 0) X.chmin_segment(l, r, {a, b});
        if (t == 1) {
            auto [fx, f] = X.query(l);
            if (fx == infty<long long>)
                print("INFINITY");
            else
                print(fx);
        }
    }
}

signed main() {
    solve();
    return 0;
}
