#ifndef AJAY_AM_SUMMAX_ADD
#define AJAY_AM_SUMMAX_ADD

#include "library/monoid/monoid_summax.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_SumMax_Add {
    using Monoid_X = Monoid_SumMax<E>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, const long long& size) {
        auto [xs, xm] = x;
        xm = (xm == numeric_limits<E>::min() / 2 ? xm : xm + a);
        return {xs + E(size) * a, xm};
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUMMAX_ADD