#ifndef AJAY_RANGE_TO_RANGE_GRAPH
#define AJAY_RANGE_TO_RANGE_GRAPH

#include "library/graph/base.hpp"

namespace mitsuha{
// https://x.com/noshi91/status/1193177214453338113
// returns directed graph, use build()
template <typename T>
struct Range_to_Range_Graph {
    int n;
    int n_node;
    vector<tuple<int, int, T>> edges;

    Range_to_Range_Graph(int n) : n(n), n_node(n * 3) {
        For(i, 2, n + n) edges.emplace_back(to_upper_idx(i / 2), to_upper_idx(i), 0); 
        For(i, 2, n + n) edges.emplace_back(to_lower_idx(i), to_lower_idx(i / 2), 0); 
    }

    void add(int frm, int to, T wt) { edges.emplace_back(frm, to, wt); }

    void add_frm_range(int frm_l, int frm_r, int to, T wt) {
        int l = frm_l + n, r = frm_r + n;
        while (l < r) {
            if (l & 1) add(to_lower_idx(l++), to, wt);
            if (r & 1) add(to_lower_idx(--r), to, wt);
            l >>= 1, r >>= 1;
        }
    }

    void add_to_range(int frm, int to_l, int to_r, T wt) {
        int l = to_l + n, r = to_r + n;
        while (l < r) {
            if (l & 1) add(frm, to_upper_idx(l++), wt);
            if (r & 1) add(frm, to_upper_idx(--r), wt);
            l >>= 1, r >>= 1;
        }
    }

    void add_range_to_range(int frm_l, int frm_r, int to_l, int to_r, T wt) {
        int new_node = n_node++;
        add_frm_range(frm_l, frm_r, new_node, wt);
        add_to_range(new_node, to_l, to_r, T(0));
    }

    Graph<T, 1> build() {
        Graph<T, 1> G(n_node);
        for (auto &&[a, b, c]: edges) G.add(a, b, c);
        G.build();
        return G;
    }

private:
    inline int to_upper_idx(const int &i) {
        if (i >= n) return i - n;
        return n + i;
    }

    inline int to_lower_idx(const int &i) {
        if (i >= n) return i - n;
        return n + n + i;
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_TO_RANGE_GRAPH