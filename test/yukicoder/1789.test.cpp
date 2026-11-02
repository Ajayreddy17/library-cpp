#define PROBLEM "https://yukicoder.me/problems/no/1789"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/shortestpath/warshall_floyd.hpp"
#include "library/graph/tree.hpp"
#include "library/flow/hungarian.hpp"

void solve() {
    INT(N1);
    Graph<int, 0> G1(N1);
    G1.read_tree();
    INT(N2);
    Graph<int, 0> G2(N2);
    G2.read_tree();

    auto dist = warshall_floyd<int>(G2);
    long long ANS = -1;

    For(r2, N2) {
        // solve with r2 as the root
        // v1, v2 -> max number of edges

        Tree<decltype(G1)> T1(G1, 0);
        Tree<decltype(G2)> T2(G2, r2);

        auto V1 = T1.V;
        auto V2 = T2.V;

        vector<vector<long long>> dp(N1, vector<long long>(N2, -infty<int>));
        Frr(idx2, N2) {
            int v2 = V2[idx2];
            auto ch2 = T2.collect_child(v2);
            int n2 = len(ch2);
            // collect vertices per direction
            vector<vector<int>> vs(n2);
            For(i, n2) {
                int c = ch2[i];
                int L = T2.LID[c], R = T2.RID[c];
                For(p, L, R) { vs[i].emplace_back(T2.V[p]); }
            }
            Frr(idx1, N1) {
                int v1 = V1[idx1];
                auto ch1 = T1.collect_child(v1);
                int n1 = len(ch1);
                if (n1 == 0) {
                    dp[v1][v2] = 0;
                    continue;
                }
                if (n1 > n2) continue;
                vector<vector<long long>> mat(n1, vector<long long>(n2, -infty<int>));
                For(i, n1) {
                    int w1 = ch1[i];
                    For(j, n2) {
                        for (auto& w2: vs[j]) {
                            chmax(mat[i][j], dp[w1][w2] + dist[v2][w2]);
                        }
                    }
                }
                auto [ans, match, X, Y] = hungarian<long long, false>(mat);
                if (ans < 0) continue;
                dp[v1][v2] = ans;
            }
        }
        chmax(ANS, dp[0][r2]);
    }
    print(ANS);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
