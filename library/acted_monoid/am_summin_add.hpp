#ifndef AJAY_AM_SUMMIN_ADD
#define AJAY_AM_SUMMIN_ADD

#include "library/monoid/monoid_summin.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_SumMin_Add {
    using Monoid_X = Monoid_SumMin<E>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, const long long& size) {
        auto [xs, xm] = x;
        xm = (xm == numeric_limits<E>::max() / 2 ? xm : xm + a);
        return {xs + E(size) * a, xm};
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUMMIN_ADD