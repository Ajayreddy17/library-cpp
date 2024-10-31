#ifndef AJAY_DISCRETE_LOG
#define AJAY_DISCRETE_LOG

#include "library/monoid/monoid_mul.hpp"
#include "library/monoid/acted_set/from_monoid.hpp"
#include "library/datastructure/hashmap.hpp"

namespace mitsuha{
// Set S acted upon by monoid X, hash function H: S -> Z
// Solve for x^n * s = t where x is in X, and s, t are in S
// Return the first solution in the range [lb, ub). If none, return -1.
template <typename ActedSet, typename F>
long long discrete_log_acted(typename ActedSet::A x, typename ActedSet::S s,
                      typename ActedSet::S t, F H, long long lb, long long ub) {
    using Mono = typename ActedSet::Monoid_A;
    using X = typename Mono::value_type;
    using S = typename ActedSet::S;

    if (lb >= ub) return -1;
    auto xpow = [&](long long n) -> X {
        X p = x;
        X res = Mono::unit();
        while (n) {
            if (n & 1) res = Mono::op(res, p);
            p = Mono::op(p, p);
            n /= 2;
        }
        return res;
    };

    auto Ht = H(t);
    s = ActedSet::act(s, xpow(lb));
    unsigned long long LIM = ub - lb;

    long long K = sqrt(LIM) + 1;

    HashMap<char> MP(K);

    For(k, K) {
        t = ActedSet::act(t, x);
        MP[H(t)] = 1;
    }

    X y = xpow(K);
    int failed = 0;
    For(k, K + 1) {
        S s1 = ActedSet::act(s, y);
        if (MP.contains(H(s1))) {
            For(i, K) {
                if (H(s) == Ht) {
                    long long ans = k * K + i + lb;
                    return (ans >= ub ? -1 : ans);
                }
                s = ActedSet::act(s, x);
            }
            if (failed) return -1;
            failed = 1;
        }
        s = s1;
    }
    return -1;
}

// Compute log_a b in group X
// Use hash function H : X -> long long
// Return the first solution in the range [lb, ub). If none, return -1.
template <typename Monoid, typename F>
long long discrete_log_monoid(typename Monoid::X a, typename Monoid::X b, F H, long long lb, long long ub) {
    using AM = ActedSet_From_Monoid<Monoid>;
    return discrete_log_acted<AM>(a, Monoid::unit(), b, H, lb, ub);
}
} // namespace mitsuha
#endif // AJAY_DISCRETE_LOG