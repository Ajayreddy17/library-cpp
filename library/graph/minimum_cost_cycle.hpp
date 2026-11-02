#ifndef AJAY_MINIMUM_COST_CYCLE
#define AJAY_MINIMUM_COST_CYCLE

#include "library/graph/base.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/minimum_cost_cycle.hpp

// {wt, vs, es}, O(N * shortest path)
template <typename T, typename GT>
tuple<T, vector<int>, vector<int>> minimum_cost_cycle_directed(GT& G) {
    const int N = G.N;
    T mi = 0, ma = 0;
    for (auto& e : G.edges) chmin(mi, e.cost), chmax(ma, e.cost);
    assert(mi >= 0);

    T ans = infty<T>;
    vector<T> dist(N);
    vector<int> vs, es;
    vector<int> par_e(N, -1);
    min_priority_queue<pair<T, int>> que;
    deque<int> deq;
    For(r, N) {
        fill(dist.begin() + r, dist.end(), infty<T>);
        if (ma <= 1) {
            auto push = [&](int v, bool back) -> void {
                (back ? deq.emplace_back(v) : deq.emplace_front(v));
            };
            for (auto& e : G[r]) {
                if (r <= e.to && chmin(dist[e.to], e.cost))
                    par_e[e.to] = e.id, push(e.to, e.cost);
            }
            while (len(deq)) {
                auto v = POP(deq);
                for (auto& e : G[v]) {
                    if (r <= e.to && chmin(dist[e.to], dist[v] + e.cost)) {
                        par_e[e.to] = e.id, push(e.to, e.cost);
                    }
                }
            }
        } else {
            for (auto& e : G[r]) {
                if (r <= e.to && chmin(dist[e.to], e.cost)) {
                    par_e[e.to] = e.id, que.emplace(e.cost, e.to);
                }
            }
            while (len(que)) {
                auto [dv, v] = POP(que);
                if (dist[v] != dv) continue;
                for (auto& e : G[v]) {
                    T x = dv + e.cost;
                    if (r <= e.to && chmin(dist[e.to], x)) {
                        par_e[e.to] = e.id, que.emplace(x, e.to);
                    }
                }
            }
        }
        if (chmin(ans, dist[r])) {
            vs.clear(), es.clear();
            vs.emplace_back(r);
            while (1) {
                int eid = par_e[vs.back()];
                es.emplace_back(eid);
                vs.emplace_back(G.edges[eid].frm);
                if (vs.back() == r) break;
            }
            reverse(vs.begin(), vs.end());
            reverse(es.begin(), es.end());
        };
    }
    return {ans, vs, es};
}

// {wt, vs, es}, O(N * shortest path)
template <typename T, typename GT>
tuple<T, vector<int>, vector<int>> minimum_cost_cycle_undirected(GT& G) {
    const int N = G.N;
    T ans = infty<T>;
    vector<T> dist(N);
    vector<int> par_e(N);
    vector<int> vs, es;
    For(r, N) {
        fill(dist.begin() + r, dist.end(), infty<T>);
        min_priority_queue<pair<T, int>> que;
        dist[r] = 0, que.emplace(0, r);
        while (len(que)) {
            auto [dv, v] = POP(que);
            if (dist[v] != dv) continue;
            for (auto& e : G[v]) {
                if (e.to < r) continue;
                T x = dv + e.cost;
                if (chmin(dist[e.to], x)) {
                    par_e[e.to] = e.id;
                    que.emplace(x, e.to);
                }
            }
        }
        int best_e = -1;
        for (auto& e : G.edges) {
            int a = e.frm, b = e.to;
            if (a < r || b < r || par_e[a] == e.id || par_e[b] == e.id) continue;
            if (chmin(ans, dist[a] + dist[b] + e.cost)) best_e = e.id;
        }
        if (best_e == -1) continue;
        vs.clear(), es.clear();
        auto& e = G.edges[best_e];
        int a = e.frm, b = e.to;
        // r -> a
        while (a != r) {
            int eid = par_e[a];
            vs.emplace_back(a), es.emplace_back(eid);
            a = G.edges[eid].frm ^ G.edges[eid].to ^ a;
        }
        vs.emplace_back(a);
        reverse(vs.begin(), vs.end()), reverse(es.begin(), es.end());
        es.emplace_back(best_e);
        while (b != r) {
            int eid = par_e[b];
            vs.emplace_back(b), es.emplace_back(eid);
            b = G.edges[eid].frm ^ G.edges[eid].to ^ b;
        }
        vs.emplace_back(b);
    }
    return {ans, vs, es};
}

// {wt, vs, es}, O(N * shortest path)
template <typename T, typename GT>
tuple<T, vector<int>, vector<int>> minimum_cost_cycle(GT& G) {
    for (auto& e : G.edges) assert(e.cost >= 0);
    if constexpr (GT::is_directed) {
        return minimum_cost_cycle_directed<T>(G);
    } else {
        return minimum_cost_cycle_undirected<T>(G);
    }
}
} // namespace mitsuha
#endif // AJAY_MINIMUM_COST_CYCLE
