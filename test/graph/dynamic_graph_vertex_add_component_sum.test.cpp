#define PROBLEM \
    "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"
#include <library/template.hpp>
#include <library/datastructure/unionfind/rollback_unionfind.hpp>
#include <library/datastructure/rollback_array.hpp>
#include <library/datastructure/offline_query/add_remove_query.hpp>

void solve() {
    LL(N, Q);
    VT(long long, A0, N);

    using P = pair<int, int>;
    Add_Remove_Query<P, 1> X;
    Rollback_Array<long long> A(A0);

    vector<int> query;
    For(Q) {
        LL(t);
        if (t == 0) {
            LL(u, v);
            if (u > v) swap(u, v);
            X.add(len(query), {u, v});
        }
        if (t == 1) {
            LL(u, v);
            if (u > v) swap(u, v);
            X.remove(len(query), {u, v});
        }
        if (t == 2) {
            LL(v, x);
            X.add(len(query), {~v, x});
        }
        if (t == 3) {
            LL(v);
            query.emplace_back(v);
        }
    }
    Rollback_UnionFind uf(N);
    // rollback_dfs
    auto upd = X.calc(len(query));
    vector<long long> ANS(len(query));
    vector<int> I(len(upd));
    iota(All(I), 0);
    auto dfs = [&](auto& dfs, vector<int>& upd_query_I, int begin, int end) -> void {
        if (begin == end) return;
        // snapshot
        int a_time = A.time();
        int uf_time = uf.time();

        vector<int> IL, IR;
        int mid = (begin + end) / 2;
        for (auto&& i: upd_query_I) {
            auto [a, b, X] = upd[i];
            if (a <= begin && end <= b) {
                // Process the update query represented by X
                auto [p, q] = X;
                if (p >= 0) {
                    int u = p, v = q;
                    u = uf[u], v = uf[v];
                    if (u == v) continue;
                    uf.merge(u, v);
                    int r = uf[u];
                    A.set(r, A.get(u) + A.get(v));
                } else {
                    int v = ~p, x = q;
                    v = uf[v];
                    A.set(v, A.get(v) + x);
                }
                continue;
            }
            if (a < mid) IL.emplace_back(i);
            if (mid < b) IR.emplace_back(i);
        }
        if (begin + 1 == end) {
            int v = query[begin];
            ANS[begin] = A.get(uf[v]);
        } else {
            dfs(dfs, IL, begin, mid);
            dfs(dfs, IR, mid, end);
        }
        // rollback
        A.rollback(a_time);
        uf.rollback(uf_time);
    };
    dfs(dfs, I, 0, len(query));
    for (auto&& x: ANS) print(x);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
