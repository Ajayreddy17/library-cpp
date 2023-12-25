#ifndef AJAY_AM_MINMAX_ADD
#define AJAY_AM_MINMAX_ADD

#include "library/monoid/monoid_minmax.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_MinMax_Add {
    using Monoid_X = Monoid_MinMax<E>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        E lo = ((x.first == numeric_limits<E>::max() / 2) ? x.first : x.first + a);
        E hi = ((x.second == numeric_limits<E>::min() / 2) ? x.second : x.second + a);
        return {lo, hi};
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MINMAX_ADD