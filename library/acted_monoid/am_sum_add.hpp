#ifndef AJAY_AM_SUM_ADD
#define AJAY_AM_SUM_ADD

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_Sum_Add {
    using Monoid_X = Monoid_Add<E>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return x + a * E(size);
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUM_ADD