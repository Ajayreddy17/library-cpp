#ifndef AJAY_AM_MAX_ADD
#define AJAY_AM_MAX_ADD

#include "library/monoid/monoid_max.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/alg/acted_monoid/max_add.hpp
template <typename E>
struct ActedMonoid_Max_Add {
    using Monoid_X = Monoid_Max<E>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return (x == numeric_limits<E>::min() / 2) ? x: x + a;
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MAX_ADD