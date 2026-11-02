#define PROBLEM "https://yukicoder.me/problems/no/1911"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/bipartite_vertex_coloring.hpp"
#include "library/util/pop.hpp"

void solve() {
    LL(N, M);
    Graph<int, 0> G(N);
    G.read_graph(M);
    vector<vector<long long>> A(2, vector<long long>(N)); read(A);

    long long ANS = 0;
    {
        auto C = bipartite_vertex_coloring(G);
        if (!C.empty()) {
            long long a = 0, b = 0;
            For(v, N) {
                if (C[v] == 0) a += A[0][v], b += A[1][v];
                if (C[v] == 1) b += A[0][v], a += A[1][v];
            }
            return print(max(a, b));
        }
    }
    // 全部良い方の色でとった場合からのロス
    long long sm = 0;
    vector<vector<long long>> loss(2, vector<long long>(N));
    For(v, N) {
        long long x = max(A[0][v], A[1][v]);
        loss[0][v] = x - A[0][v];
        loss[1][v] = x - A[1][v];
        sm += x;
    }

    // ロス最小の交互サイクルを探す
    For(S, N) {
        For(Sc, 2) {
            vector<vector<long long>> dist(2, vector<long long>(N, infty<long long>));
            min_priority_queue<tuple<long long, int, int>> que;
            auto add = [&](long long c, long long v, long long d) -> void {
                if (chmin(dist[c][v], d)) que.emplace(d, c, v);
            };
            add(Sc, S, loss[Sc][S]);
            while (len(que)) {
                auto [d, c, v] = POP(que);
                if (d != dist[c][v]) continue;
                for (auto&& e : G[v]) {
                    add(1 - c, e.to, dist[c][v] + loss[1 - c][e.to]);
                }
            }
            chmax(ANS, sm - dist[1 - Sc][S] + loss[1 - Sc][S]);
        }
    }
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
