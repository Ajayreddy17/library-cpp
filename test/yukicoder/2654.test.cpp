#define PROBLEM "https://yukicoder.me/problems/no/2654"
#include "library/template.hpp"
#include "library/datastructure/slide_split_sum.hpp"
#include "library/graph/base.hpp"

void solve() {
    LL(N);
    ++N;
    vector<long long> A(N); read(A);
    Graph<int, 0> G(N);
    G.read_tree(0, 0);

    Removable_Queue<min_priority_queue<long long>> que1;
    Removable_Queue<max_priority_queue<long long>> que2;

    Slide_Split_Sum<long long> X;

    auto F = [&]() -> long long {
        long long n = len(X);
        if (n <= 2) return -1;
        long long mi = que1.top();
        long long ma = que2.top();

        long long ANS = linf;

        if (n % 2 == 0) {
            long long k = (n - 2) / 2;
            {
                long long a = X.query_l(k);
                long long b = X.query_l(k + 1) - a;
                long long c = X.query_r(k + 1) - ma;
                long long ans = (b * k - a) + (c - b * k);
                if (b == ma) ++ans;
                chmin(ANS, ans);
            }
            {
                long long a = X.query_l(k + 1) - mi;
                long long c = X.query_r(k);
                long long b = X.query_r(k + 1) - c;
                long long ans = (b * k - a) + (c - b * k);
                if (b == mi) ++ans;
                chmin(ANS, ans);
            }
            return ANS;
        }
        assert(n % 2 == 1);
        long long k = (n - 3) / 2;
        {
            long long a = X.query_l(k + 1) - mi;
            long long c = X.query_r(k + 1);
            long long b = X.query_r(k + 2) - c;
            long long ans = (b * k - a) + (c - b * (k + 1));
            if (b == mi) ++ans;
            chmin(ANS, ans);
        }
        {
            long long a = X.query_l(k);
            long long b = X.query_l(k + 1) - a;
            long long c = X.query_r(k + 2) - ma;
            long long ans = (b * k - a) + (c - b * (k + 1));
            if (b == ma) ++ans;
            chmin(ANS, ans);
        }
        return ANS;
    };

    vector<long long> ANS(N);
    auto dfs = [&](auto& dfs, int v, int p) -> void {
        X.insert(A[v]);
        que1.push(A[v]);
        que2.push(A[v]);
        for (auto& e : G[v]) {
            if (e.to == p) continue;
            dfs(dfs, e.to, v);
        }
        ANS[v] = F();
        X.erase(A[v]);
        que1.remove(A[v]);
        que2.remove(A[v]);
    };
    dfs(dfs, 0, -1);

    For(v, 1, N) { print(ANS[v]); }
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
