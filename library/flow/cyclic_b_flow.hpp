#ifndef AJAY_CYCLIC_B_FLOW
#define AJAY_CYCLIC_B_FLOW

#include "library/util/binary_serch.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/cyclic_b_flow.hpp
/*
Min cost flow on a cycle 0 -> 1 -> ... -> N-1 -> 0.
supply[i]: supply (+) / demand (-) at i, sum = 0
cost[i]: cost per unit on edge i -> i+1 (both directions)
https://atcoder.jp/contests/dwango2015-prelims/tasks/dwango2015_prelims_4
*/
template <typename CAP, typename COST>
COST cyclic_b_flow(vector<CAP> supply, vector<COST> cost) {
    const int N = len(supply);
    if (N == 0) return 0;
    assert(accumulate(supply.begin(), supply.end(), CAP(0)) == 0);
    auto f = [&](long long x) -> long long {
        // x = flow on edge N-1 -> 0
        long long res = abs(x) * cost.back();
        For(i, N - 1) {
            // flow leaving i to the right
            x += supply[i];
            res += abs(x) * cost[i];
        }
        return res;
    };

    auto check = [&](long long x) -> bool { return f(x) <= f(x + 1); };
    long long LIM = 5;
    for (auto&& x : supply) LIM += max<long long>(x, 0);
    long long x = binary_search<FirstTrue>(-LIM, LIM, check);
    return f(x);
}
} // namespace mitsuha
#endif // AJAY_CYCLIC_B_FLOW
