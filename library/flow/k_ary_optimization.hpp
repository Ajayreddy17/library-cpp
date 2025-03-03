#ifndef AJAY_K_ARY_OPTIMIZATION
#define AJAY_K_ARY_OPTIMIZATION

#include "library/flow/maxflow.hpp"

namespace mitsuha{
// ABC347G
template <typename T, bool MINIMIZE>
struct K_ary_Optimization {
    int n;
    vector<int> ks;
    vector<vector<int>> idx;
    map<pair<int, int>, T> edges;
    int source, sink, nxt;
    T base_cost;

    // idx[i][j] is the source side of cut: val[i]>=j
    K_ary_Optimization(vector<int> ks) : n(len(ks)), ks(ks), base_cost(0) {
        source = 0, sink = 1, nxt = 2;
        for (auto& k : ks) {
            assert(k >= 1);
            vector<int> I(k + 1);
            I[0] = source, I[k] = sink;
            For(i, 1, k) { I[i] = nxt++; }
            idx.emplace_back(I);
            if (k >= 3) {
                For(j, 1, k - 1) add_edge(I[j + 1], I[j], numeric_limits<T>::max() / 2);
            }
        }
    }

    // Add the cost of setting xi to 0, 1, ..., k-1.
    void add_1(int i, vector<T> cost) {
        assert(0 <= i && i < n && len(cost) == ks[i]);
        if (!MINIMIZE) {
            for (auto& x : cost)
                x = -x;
        }
        _add_1(i, cost);
    }

    void add_2(int i, int j, vector<vector<T>> cost) {
        assert(0 <= i && i < n && 0 <= j && j < n && i != j);
        int H = ks[i], W = ks[j];
        assert(len(cost) == H);
        For(a, H) assert(len(cost[a]) == W);
        if (!MINIMIZE) {
            For(a, H) For(b, W) cost[a][b] = -cost[a][b];
        }
        _add_2(i, j, cost);
    }

    // Returns cost and the [0,k) sequence
    pair<T, vector<int>> calc() {
        MaxFlow<T> G(nxt, source, sink);
        for (auto&& [key, cap] : edges) {
            auto [frm, to] = key;
            G.add(frm, to, cap);
        }

        auto [val, cut] = G.cut();
        val += base_cost;
        vector<int> ANS(n);
        For(i, n) {
            For(j, 1, ks[i]) { ANS[i] += 1 - cut[idx[i][j]]; }
        }
        if (!MINIMIZE)
            val = -val;
        return {val, ANS};
    }

  private:
    void add_base(T x) {
        base_cost += x;
        assert(-numeric_limits<T>::max() / 2 < base_cost && base_cost < numeric_limits<T>::max() / 2);
    }

    void add_edge(int i, int j, T t) {
        assert(t >= 0);
        if (t == 0)
            return;
        pair<int, int> key = make_pair(i, j);
        edges[key] += t;
        assert(edges[key] <= numeric_limits<T>::max() / 2);
    }

    void _add_1(int i, vector<T> cost) {
        add_base(cost[0]);
        Frr(j, ks[i]) cost[j] -= cost[0];
        For(j, 1, ks[i]) {
            T x = cost[j] - cost[j - 1];
            // j or more, x
            if (x > 0)
                add_edge(idx[i][j], sink, x);
            if (x < 0)
                add_base(x), add_edge(source, idx[i][j], -x);
        }
    }
    void _add_2(int i, int j, vector<vector<T>> cost) {
        int H = ks[i], W = ks[j];
        _add_1(j, cost[0]);
        Frr(a, H) For(b, W) cost[a][b] -= cost[0][b];
        vector<T> tmp(H);
        For(a, H) tmp[a] = cost[a][W - 1];
        _add_1(i, tmp);
        For(a, H) For(b, W) cost[a][b] -= tmp[a];
        For(a, 1, H) For(b, W - 1) {
            T x = cost[a][b] + cost[a - 1][b + 1] - cost[a - 1][b] - cost[a][b + 1];
            assert(x >= 0); // monge
            add_edge(idx[i][a], idx[j][b + 1], x);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_K_ARY_OPTIMIZATION