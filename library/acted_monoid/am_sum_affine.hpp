#ifndef AJAY_AM_SUM_AFFINE
#define AJAY_AM_SUM_AFFINE

#include "library/monoid/monoid_add.hpp"
#include "library/monoid/monoid_affine.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_Sum_Affine {
    using Monoid_X = Monoid_Add<E>;
    using Monoid_A = Monoid_Affine<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return x * a.first + E(size) * a.second;
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUM_AFFINE
