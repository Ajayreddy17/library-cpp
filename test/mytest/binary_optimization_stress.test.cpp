#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/flow/binary_optimization.hpp"
#include "library/flow/k_ary_optimization.hpp"

// random submodular terms; compare calc() with brute force over all assignments
template <bool MINIMIZE>
void test_binary() {
    For(iter, 3000) {
        int n = RNG(1, 7);
        Binary_Optimization<long long, MINIMIZE> BO(n);
        vector<function<long long(int)>> terms;  // cost of an assignment mask (MINIMIZE sign)
        int sg = (MINIMIZE ? 1 : -1);
        For(RNG(0, 6)) {
            int i = RNG(0, n);
            long long a = RNG(-9, 10), b = RNG(-9, 10);
            BO.add_1(i, sg * a, sg * b);
            terms.push_back([=](int m) { return (m >> i & 1) ? b : a; });
        }
        if (n >= 2) For(RNG(0, 6)) {
            int i = RNG(0, n), j = RNG(0, n);
            if (i == j) continue;
            long long x01 = RNG(-9, 10), x10 = RNG(-9, 10), x00 = RNG(-9, 10);
            long long x11 = x01 + x10 - x00 - RNG(0, 6);
            BO.add_2(i, j, sg * x00, sg * x01, sg * x10, sg * x11);
            terms.push_back([=](int m) {
                int a = m >> i & 1, b = m >> j & 1;
                return a ? (b ? x11 : x10) : (b ? x01 : x00);
            });
        }
        if (n >= 3) For(RNG(0, 4)) {
            int i = RNG(0, n), j = RNG(0, n), k = RNG(0, n);
            if (i == j || i == k || j == k) continue;
            // submodular: concave in the count + pairwise submodular parts
            long long g[4];
            g[0] = 0, g[1] = RNG(-9, 10), g[2] = 2 * g[1] - RNG(0, 6), g[3] = 2 * g[2] - g[1] - RNG(0, 6);
            long long u[3] = {RNG(-5, 6), RNG(-5, 6), RNG(-5, 6)};
            long long x[8];
            For(m, 8) {
                int a = m >> 2 & 1, b = m >> 1 & 1, c = m & 1;
                x[m] = g[a + b + c] + u[0] * a + u[1] * b + u[2] * c;
            }
            BO.add_3(i, j, k, sg * x[0], sg * x[1], sg * x[2], sg * x[3], sg * x[4], sg * x[5], sg * x[6], sg * x[7]);
            terms.push_back([=](int m) {
                int a = m >> i & 1, b = m >> j & 1, c = m >> k & 1;
                return x[4 * a + 2 * b + c];
            });
        }
        auto cost = [&](int m) { long long s = 0; for (auto& f: terms) s += f(m); return s; };
        long long best = linf;
        For(m, 1 << n) chmin(best, cost(m));
        auto [val, X] = BO.calc();
        assert(val == sg * best);
        int m = 0;
        For(i, n) m |= X[i] << i;
        assert(cost(m) == best);
    }
}

void test_k_ary() {
    For(iter, 2000) {
        int n = RNG(1, 4);
        vector<int> ks(n);
        For(i, n) ks[i] = RNG(1, 4);
        K_ary_Optimization<long long, true> KO(ks);
        vector<vector<long long>> c1(n);
        For(i, n) {
            c1[i].resize(ks[i]);
            For(a, ks[i]) c1[i][a] = RNG(-9, 10);
            KO.add_1(i, c1[i]);
        }
        // Monge pair costs: c[a][b] = f(a - b) with f convex
        vector<tuple<int, int, vector<vector<long long>>>> c2;
        For(RNG(0, 4)) {
            int i = RNG(0, n), j = RNG(0, n);
            if (i == j) continue;
            long long p = RNG(0, 4), q = RNG(-3, 4);
            vector<vector<long long>> c(ks[i], vector<long long>(ks[j]));
            For(a, ks[i]) For(b, ks[j]) c[a][b] = p * (a - b) * (a - b) + q * (a - b);
            KO.add_2(i, j, c);
            c2.emplace_back(i, j, c);
        }
        auto cost = [&](const vector<int>& x) {
            long long s = 0;
            For(i, n) s += c1[i][x[i]];
            for (auto& [i, j, c]: c2) s += c[x[i]][x[j]];
            return s;
        };
        long long best = linf;
        vector<int> x(n);
        auto rec = [&](auto& rec, int i) -> void {
            if (i == n) { chmin(best, cost(x)); return; }
            For(a, ks[i]) x[i] = a, rec(rec, i + 1);
        };
        rec(rec, 0);
        auto [val, X] = KO.calc();
        assert(val == best && cost(X) == best);
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_binary<true>();
    test_binary<false>();
    test_k_ary();
    solve();
    return 0;
}
