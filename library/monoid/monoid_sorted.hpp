#ifndef AJAY_MONOID_SORTED
#define AJAY_MONOID_SORTED

namespace mitsuha{
template <class T>
struct Monoid_Sorted {
    using value_type = tuple<bool, bool, T, T>;
    using X = value_type;
    static constexpr X op(const X &x, const X &y) noexcept {
        if (get<1>(x)) return y;
        if (get<1>(y)) return x;
        if (get<3>(x) <= get<2>(y)) return make_tuple(bool(get<0>(x) & get<0>(y)), false, get<2>(x), get<3>(y));
        return make_tuple(false, false, T(), T());
    }
    static constexpr X from_element(const T &x) { return make_tuple(true, false, x, x); }
    static constexpr X unit() { return make_tuple(true, true, 0, 0); }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_SORTED
