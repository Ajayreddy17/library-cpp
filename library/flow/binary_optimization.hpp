#ifndef AJAY_BINARY_OPTIMIZATION
#define AJAY_BINARY_OPTIMIZATION

#include "library/flow/maxflow.hpp"

namespace mitsuha{
template <typename T, bool MINIMIZE>
struct Binary_Optimization {
    int n;
    int nxt;
    int source, sink;
    T base_cost;
    map<pair<int, int>, T> edges;

    Binary_Optimization(int n) : n(n), base_cost(0) {
        source = n;
        sink = n + 1;
        nxt = n + 2;
    }

    // Add the cost of setting xi to 0 or 1.
    void add_1(int i, T x0, T x1) {
        assert(0 <= i && i < n);
        if (!MINIMIZE) {
            x0 = -x0, x1 = -x1;
        }
        _add_1(i, x0, x1);
    }

    // Add the cost of setting (xi,xj) = (00,01,10,11).
    // The cost must be x00 + x11 <= x10 + x10 for the MINIMUM model.
    // In particular, we can give a gain to the diagonal elements.。
    void add_2(int i, int j, T x00, T x01, T x10, T x11) {
        assert(i != j);
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);
        if (!MINIMIZE) {
            x00 = -x00, x01 = -x01;
            x10 = -x10, x11 = -x11;
        }
        _add_2(i, j, x00, x01, x10, x11);
    }

    // (xi,xj,xk) = (000,001,010,011,100,101,110,111)
    // Add the cost of making it so. It needs to be MINIMUM.
    // In particular, you can give a benefit to 000 and 111.
    void add_3(int i, int j, int k, T x000, T x001, T x010, T x011, T x100, T x101,
               T x110, T x111) {
        assert(i != j && i != k && j != k);
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);
        assert(0 <= k && k < n);
        if (!MINIMIZE) {
            x000 = -x000, x001 = -x001;
            x010 = -x010, x011 = -x011;
            x100 = -x100, x101 = -x101;
            x110 = -x110, x111 = -x111;
        }
        _add_3(i, j, k, x000, x001, x010, x011, x100, x101, x110, x111);
    }

    // Returns the minimum value and the 01 column
    pair<T, vector<int>> calc() {
        MaxFlow<T> G(nxt, source, sink);
        for (auto&& [key, cap] : edges) {
            auto [frm, to] = key;
            G.add(frm, to, cap);
        }
        auto [val, cut] = G.cut();
        val += base_cost;
        chmin(val, numeric_limits<T>::max() / 2);
        cut.resize(n);
        if (!MINIMIZE)
            val = -val;
        return {val, cut};
    }

private:
    void add_base(T t) {
        base_cost += t;
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

    void _add_1(int i, T x0, T x1) {
        if (x0 <= x1) {
            add_base(x0);
            add_edge(source, i, x1 - x0);
        } else {
            add_base(x1);
            add_edge(i, sink, x0 - x1);
        }
    }

    void _add_2(int i, int j, T x00, T x01, T x10, T x11) {
        assert(x00 + x11 <= x01 + x10);
        _add_1(i, x00, x10);
        _add_1(j, 0, x11 - x10);
        add_edge(i, j, x01 + x10 - x00 - x11);
    }

    void _add_3(int i, int j, int k, T x000, T x001, T x010, T x011, T x100, T x101,
                T x110, T x111) {
        T p = x000 - x100 - x010 - x001 + x110 + x101 + x011 - x111;
        if (p > 0) {
            add_base(x000);
            _add_1(i, 0, x100 - x000);
            _add_1(j, 0, x010 - x000);
            _add_1(k, 0, x001 - x000);
            _add_2(i, j, 0, 0, 0, x000 + x110 - x100 - x010);
            _add_2(i, k, 0, 0, 0, x000 + x101 - x100 - x001);
            _add_2(j, k, 0, 0, 0, x000 + x011 - x010 - x001);
            // Now, add the gain p when the value is 111.
            add_base(-p);
            // Other than 111, the cost p
            add_edge(i, nxt, p);
            add_edge(j, nxt, p);
            add_edge(k, nxt, p);
            add_edge(nxt, sink, p);
            ++nxt;
        } else {
            p = -p;
            add_base(x111);
            _add_1(i, x011 - x111, 0);
            _add_1(i, x101 - x111, 0);
            _add_1(i, x110 - x111, 0);
            _add_2(i, j, x111 + x001 - x011 - x101, 0, 0, 0);
            _add_2(i, k, x111 + x010 - x011 - x110, 0, 0, 0);
            _add_2(j, k, x111 + x100 - x101 - x110, 0, 0, 0);
            // 000 Add a payoff p when
            add_base(-p);
            // If it is other than 000, the cost p
            add_edge(nxt, i, p);
            add_edge(nxt, j, p);
            add_edge(nxt, k, p);
            add_edge(source, nxt, p);
            ++nxt;
        }
    }
};

template <typename T, bool MINIMIZE>
ostream &operator<<(ostream &out, const Binary_Optimization<T, MINIMIZE> &BO){
    auto Bo = BO;
    out << "base cost " << Bo.base_cost << "\n";
    out << "source " << Bo.source << " sink " << Bo.sink << "\n";
    for (auto&& [key, cap] : Bo.edges)
        out << key << " " << cap;
    return out;
}
} // namespace mitsuha
#endif // AJAY_BINARY_OPTIMIZATION