#ifndef AJAY_MONOID_ADD_PAIR
#define AJAY_MONOID_ADD_PAIR

namespace mitsuha{
template <typename E>
struct Monoid_Add_Pair {
    using value_type = pair<E, E>;
    using X = value_type;
    static constexpr X op(const X &x, const X &y) {
        return make_pair(x.first + y.first, x.second + y.second);
    }
    static constexpr X inverse(const X &x) { return make_pair(-x.first, -x.second); }
    static constexpr X unit() { return make_pair(0, 0); }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ADD_PAIR