#ifndef AJAY_AM_MAXIDX_ADD
#define AJAY_AM_MAXIDX_ADD

#include "library/monoid/monoid_maxidx.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename E, bool tie_is_left = true>
struct ActedMonoid_MaxIdx_Add {
    using Monoid_X = Monoid_Max_Idx<E, tie_is_left>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        if (x.first == numeric_limits<E>::min() / 2) return x;
        return {x.first + a, x.second};
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MAXIDX_ADD