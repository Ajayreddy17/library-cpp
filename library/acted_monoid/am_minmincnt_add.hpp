#ifndef AJAY_AM_MINMINCNT_ADD
#define AJAY_AM_MINMINCNT_ADD

#include "library/monoid/monoid_minmincnt.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_MinMincnt_Add {
    using Monoid_X = Monoid_MinMincnt<E>;
    using Monoid_A = Monoid_Add<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        auto [xmin, xmincnt] = x;
        if (xmin == numeric_limits<E>::max() / 2) return x;
        return make_pair(xmin + a, xmincnt);
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MINMINCNT_ADD