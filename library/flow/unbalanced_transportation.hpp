#ifndef AJAY_UNBALANCED_TRANSPORTATION
#define AJAY_UNBALANCED_TRANSPORTATION

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/unbalanced_transportation.hpp
/*
Bipartite min cost flow with K left vertices, N right vertices and flow N,
in O((K^2 log N + K^3) N).
https://ac.nowcoder.com/acm/contest/33188/B
https://qoj.ac/contest/1356/problem/7185
*/
template <typename T>
struct Unbalanced_Transportation_Problem {
    int K, N;
    vector<int> supply;
    vector<vector<T>> cost;
    vector<int> FRM;
    Unbalanced_Transportation_Problem(int K, int N, vector<int>& supply)
            : K(K), N(N), supply(supply) {
        assert(accumulate(supply.begin(), supply.end(), 0LL) >= N);
        cost.assign(K, vector<T>(N, numeric_limits<T>::max() / 2));
        FRM.assign(N, -1);
    }

    void add(int a, int b, T c) {
        assert(0 <= a && a < K);
        assert(0 <= b && b < N);
        cost[a][b] = c;
    }

    // flow -> cost
    vector<T> calc() {
        using Q = min_priority_queue<pair<T, int>>;
        vector<T> res = {0};
        T MCF = 0;
        vector<vector<Q>> edge(K, vector<Q>(K));
        vector<Q> unused(K);
        For(a, K) For(b, N) {
            T c = cost[a][b];
            if (c == numeric_limits<T>::max() / 2) continue;
            unused[a].emplace(c, b);
        }
        while (1) {
            if (len(res) == N + 1) break;
            // update prique
            For(a, K) {
                auto& que = unused[a];
                while (len(que)) {
                    auto [x, b] = que.top();
                    if (FRM[b] == -1) break;
                    que.pop();
                }
            }
            For(a, K) For(b, K) {
                auto& que = edge[a][b];
                while (len(que)) {
                    auto [x, c] = que.top();
                    if (FRM[c] == b) {
                        break;
                    }
                    que.pop();
                }
            }

            // build the graph
            vector<vector<T>> dist_0(K, vector<T>(K, numeric_limits<T>::max() / 2));
            For(a, K) For(b, K) {
                if (len(edge[a][b]) == 0) continue;
                auto [c, idx] = edge[a][b].top();
                dist_0[a][b] = c;
            }
            // shortest paths from source
            vector<T> dist(K, numeric_limits<T>::max() / 2);
            vector<bool> in_que(K);
            queue<int> que;
            vector<int> par(K, -1);
            For(k, K) {
                if (supply[k]) {
                    dist[k] = 0, in_que[k] = 1, que.emplace(k);
                }
            }
            while (!que.empty()) {
                int v = que.front();
                que.pop(), in_que[v] = 0;
                For(to, K) {
                    if (dist_0[v][to] == numeric_limits<T>::max() / 2) continue;
                    if (chmin(dist[to], dist[v] + dist_0[v][to])) {
                        par[to] = v;
                        if (!in_que[to]) {
                            in_que[to] = 1;
                            que.emplace(to);
                        }
                    }
                }
            }
            int best = -1;
            T best_c = numeric_limits<T>::max() / 2;
            For(k, K) {
                if (dist[k] == numeric_limits<T>::max() / 2) continue;
                T x = dist[k] + unused[k].top().first;
                if (chmin(best_c, x)) best = k;
            }
            if (best == -1) break;
            auto match = [&](int a, int b) -> void {
                int c = FRM[b];
                if (c != -1) MCF -= cost[c][b];
                FRM[b] = a;
                MCF += cost[a][b];
                For(v, K) {
                    if (v == a || cost[v][b] == numeric_limits<T>::max() / 2) continue;
                    edge[v][a].emplace(cost[v][b] - cost[a][b], b);
                }
            };
            int v = best;
            match(v, unused[v].top().second);
            while (par[v] != -1) {
                match(par[v], edge[par[v]][v].top().second);
                v = par[v];
            }
            supply[v]--;
            res.emplace_back(MCF);
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_UNBALANCED_TRANSPORTATION
