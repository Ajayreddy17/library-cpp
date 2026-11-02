#ifndef AJAY_LONGEST_SHORTEST_PATH
#define AJAY_LONGEST_SHORTEST_PATH

#include "library/flow/mincostflow.hpp"
#include "library/graph/shortestpath/dijkstra.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/longest_shortest_path.hpp

/*
potential p[v]
want dist >= L : L <= p[t] - p[s]
edge (u, v, w)
extension needed: max(0, p[v] - p[u] - w)
- t -> s with cost -L, capacity INF
- u -> v with cost w, capacity 1
min cost circulation of this graph
*/

// https://qoj.ac/contest/1435/problem/7737
template <typename T = long long, bool DAG = false>
struct Longest_Shortest_Path {
    int N, s, t;
    T F, L, K;
    bool solved;
    vector<tuple<int, int, T, T>> dat;
    vector<T> pot;
    Longest_Shortest_Path(int N, int s, int t): N(N), s(s), t(t), F(0), solved(0) {}

    // current length, cost to add +1 to the length
    void add(int frm, int to, T length, T cost) {
        assert(0 <= frm && frm < N && 0 <= to && to < N && !solved);
        if (DAG) assert(frm < to);
        dat.emplace_back(frm, to, length, cost);
    }

    T init_dist() {
        Graph<T, 1> G(N);
        for (auto& [a, b, c, d] : dat) G.add(a, b, c);
        G.build();
        auto [dist, par] = dijkstra<T>(G, s);
        return dist[t];
    }

    // make dist(s, t) >= L. return: min cost.
    T solve_by_target_length(T target_length) {
        L = target_length;
        assert(!solved && L >= init_dist());
        solved = 1;
        Min_Cost_Flow<T, T, DAG> G(N, s, t);
        for (auto& [a, b, length, cost] : dat) {
            G.add(a, b, cost, length);
        }
        T ans = -infty<T>;
        for (auto& [x, y] : G.slope()) {
            if (chmax(ans, x * L - y)) F = x;
        }
        return K = ans;
    }

    // maximize dist(s, t) with total cost <= K. return: max dist (infty<T> if unbounded).
    // cost(L) = max_i (x_i L - y_i) over the slope points (x_i, y_i), so
    // max L = min_{x_i > 0} floor((K + y_i) / x_i).
    T solve_by_cost(T budget) {
        assert(!solved && budget >= 0);
        Min_Cost_Flow<T, T, DAG> G(N, s, t);
        for (auto& [a, b, length, cost] : dat) {
            G.add(a, b, cost, length);
        }
        auto S = G.slope();
        T best = infty<T>;
        for (auto& [x, y] : S) {
            if (x > 0) chmin(best, fld<T>(budget + y, x));
        }
        if (best == infty<T>) return best;
        solved = 1, L = best, K = -infty<T>;
        for (auto& [x, y] : S) {
            if (chmax(K, x * L - y)) F = x;
        }
        return L;
    }

    // potentials p after solving; dist(s, v) in the extended graph
    vector<T> get_potentials() {
        assert(solved);
        if (len(pot)) return pot;
        Min_Cost_Flow<T, T, DAG> G(N, s, t);
        for (auto& [a, b, length, cost] : dat) {
            G.add(a, b, cost, length);
        }
        G.flow(F);
        pot = G.get_potentials();
        Graph<T, 1> resG(N);
        auto add = [&](int a, int b, T x) -> void {
            x = x + pot[a] - pot[b];
            resG.add(a, b, x);
        };
        for (auto& e : G.edges()) {
            if (e.cap > e.flow) add(e.frm, e.to, e.cost);
            if (e.flow > 0) add(e.to, e.frm, -e.cost);
        }
        add(s, t, L), add(t, s, -L);
        resG.build();
        vector<T> dist = dijkstra<T>(resG, s).first;
        For(x, N) pot[x] += dist[x];
        return pot;
    }

    // edge lengths after extension
    vector<T> get_edges() {
        get_potentials();
        vector<T> res;
        for (auto [frm, to, length, cost] : dat) {
            res.emplace_back(max<T>(length, pot[to] - pot[frm]));
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_LONGEST_SHORTEST_PATH
