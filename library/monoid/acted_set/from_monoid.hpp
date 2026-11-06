#ifndef AJAY_ACTEDSET_FROM_MONOID
#define AJAY_ACTEDSET_FROM_MONOID

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/alg/acted_set/from_monoid.hpp
template <typename Monoid>
struct ActedSet_From_Monoid {
    using Monoid_A = Monoid;
    using A = typename Monoid::value_type;
    using S = A;
    static S act(const S &x, const A &g) { return Monoid::op(x, g); }
};
} // namespace mitsuha
#endif // AJAY_ACTEDSET_FROM_MONOID