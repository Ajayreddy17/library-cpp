#ifndef AJAY_DOUBLING
#define AJAY_DOUBLING

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
// After one operation from state a, 
// transition to state b and add monoid element x.
// If there is no destination: -1 (add not required)
template <typename Monoid, int LOG>
struct Doubling {
    using X = typename Monoid::value_type;
    int N;
    bool is_prepared;
    vector<vector<int>> TO;
    vector<vector<X>> DP;

    Doubling(int N) : N(N), is_prepared(0) {
        TO.assign(LOG, vector<int>(N, -1));
        DP.assign(LOG, vector<X>(N, Monoid::unit()));
    }

    void add(int i, int to, X x) {
        Assert(!is_prepared);
        Assert(-1 <= to && to < N);
        TO[0][i] = to;
        DP[0][i] = x;
    }

    void build() {
        Assert(!is_prepared);
        is_prepared = 1;
        For(k, LOG - 1) {
            For(v, N) {
                int w = TO[k][v];
                if (w == -1) {
                  TO[k + 1][v] = -1;
                  DP[k + 1][v] = DP[k][v];
                  continue;
                }
                TO[k + 1][v] = TO[k][w];
                DP[k + 1][v] = Monoid::op(DP[k][v], DP[k][w]);
            }
        }
    }

    // (to, val)
    pair<int, X> calc(int i, long long step) {
        Assert(is_prepared);
        assert(0 <= step && step < (1LL << LOG));
        X x = Monoid::unit();
        while (step && i != -1) {
            int k = (step == 0 ? -1 : 63 - __builtin_clzll(step));
            step ^= 1LL << k;
            x = Monoid::op(x, DP[k][i]);
            i = TO[k][i];
        }
        return {i, x};
    }

    // check(to, monoid_sum)
    template <typename F>
    long long max_step(int i, F check) {
        Assert(is_prepared);
        X x = Monoid::unit();
        long long step = 0;
        assert(check(i, x));
        Frr(k, LOG) {
            int j = TO[k][i];
            if (j == -1) continue;
            X y = Monoid::op(x, DP[k][i]);
            if (check(j, y)) {
                step |= 1LL << k;
                i = j;
                x = y;
                assert(i != -1);
            }
        }
        return step;
    }
};

template<typename Monoid, int LOG>
std::ostream &operator<<(std::ostream &out, const Doubling<Monoid, LOG> &_D){
    auto D = _D;
    out << "[";
    out << "\nTO\n";
    For(k, LOG) out << D.TO[k] << "\n";
    out << "DP\n";
    For(k, LOG) out << D.DP[k] << "\n";
    return out << ']';
}
} // namespace mitsuha
#endif // AJAY_DOUBLING