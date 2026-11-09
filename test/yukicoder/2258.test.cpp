#define PROBLEM "https://yukicoder.me/problems/no/2258"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree.hpp"
#include "library/graph/datastructure/static_toptree.hpp"

void solve() {
    LL(N);
    Graph<int, 0> G(N);
    G.read_tree(0, 0);
    VT(int, A, N);

    Tree<decltype(G)> tree(G);
    Static_TopTree<decltype(tree)> STT(tree);

    // merge two sorted lists; left[i] = 1 if C[i] came from A
    auto merge = [&](vector<int>& A, vector<int>& B) -> pair<vector<int>, vector<int>> {
        vector<int> C;
        vector<int> left;
        int a = 0, b = 0;
        A.emplace_back(iinf), B.emplace_back(iinf);
        For(len(A) + len(B) - 2) {
            if (A[a] < B[b]) {
                C.emplace_back(A[a++]), left.emplace_back(1);
            } else {
                C.emplace_back(B[b++]), left.emplace_back(0);
            }
        }
        A.pop_back(), B.pop_back();
        return {C, left};
    };

    int n = len(STT.par);
    vector<vector<int>> V(n);
    vector<vector<int>> LEFT(n);
    vector<vector<long long>> Ac(n);
    {
        auto dfs = [&](auto& dfs, int k) -> void {
            if (k < N) {
                V[k] = {k};
                return;
            }
            int l = STT.lch[k], r = STT.rch[k];
            dfs(dfs, l), dfs(dfs, r);
            auto [C, left] = merge(V[l], V[r]);
            V[k] = C;
            LEFT[k] = left;
        };
        dfs(dfs, 2 * N - 2);
    }

    For(i, n) {
        Ac[i] = {0};
        for (auto& v: V[i]) Ac[i].emplace_back(Ac[i].back() + A[v]);
        vector<int> cs = {0};
        for (auto& x: LEFT[i]) cs.emplace_back(cs.back() + x);
        LEFT[i] = cs;
    }

    auto &lch = STT.lch, &rch = STT.rch;
    auto solve = [&](long long L, long long R, long long delta, long long K) -> int {
        auto get = [&](int k, long long L, long long R, long long delta) -> long long {
            // the [L, R)-th elements of this cluster's sorted vertex list,
            // total weight with weights K + A[v]
            long long cnt = R - L;
            long long sm = Ac[k][R] - Ac[k][L];
            long long val = K * cnt + sm;
            val = 10 * val + (delta != -1);
            return val;
        };
        long long total = get(2 * N - 2, L, R, delta);
        long long large = cld<long long>(total, 2);
        auto dfs = [&](auto& dfs, int k, long long L, long long R, long long d, long long cu, long long cv) -> int {
            if (cu >= large || cv >= large) return -1;
            if (k < N) { return k; }
            long long L1 = LEFT[k][L], R1 = LEFT[k][R];
            long long L2 = L - L1, R2 = R - R1;
            long long d1 = -1, d2 = -1;
            if (d != -1) {
                if (LEFT[k][d] < LEFT[k][d + 1]) {
                    d1 = LEFT[k][d];
                } else {
                    d2 = d - LEFT[k][d];
                }
            }
            int l = lch[k], r = rch[k];
            long long a = get(l, L1, R1, d1), b = get(r, L2, R2, d2);
            assert(cu + cv + a + b == total);
            if (STT.is_compress[k]) {
                if (cu + a > cv + b) {
                    return dfs(dfs, l, L1, R1, d1, cu, cv + b);
                } else {
                    return dfs(dfs, r, L2, R2, d2, cu + a, cv);
                }
            }
            if (STT.B[k] == -1) {
                if (a > b) {
                    return dfs(dfs, l, L1, R1, d1, cu + b, 0);
                } else {
                    return dfs(dfs, r, L2, R2, d2, cu + a, 0);
                }
            }
            if (a + cv >= large) { return dfs(dfs, l, L1, R1, d1, cu + b, cv); }
            int kk = dfs(dfs, r, L2, R2, d2, cu + cv + a, 0);
            return (kk == -1 ? STT.A[k] : kk);
        };
        return dfs(dfs, 2 * N - 2, L, R, delta, 0, 0);
    };

    long long X_SUM = 0;
    INT(Q);
    For(Q) {
        LL(aa, bb, kk, delta);
        long long a = (aa + X_SUM) % N;
        long long b = (bb + 2 * X_SUM) % N;
        int mod = 150001;
        long long K = (kk + (X_SUM % mod) * (X_SUM % mod)) % mod;
        long long L = min(a, b);
        long long R = 1 + max(a, b);

        long long X = solve(L, R, delta, K);
        print(X);
        X_SUM += X;
    }
}

signed main() {
    solve();
    return 0;
}
