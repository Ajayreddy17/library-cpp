#ifndef AJAY_MONOID_MINIDX
#define AJAY_MONOID_MINIDX

namespace mitsuha{
template <typename T, bool tie_is_left = true>
struct Monoid_Min_Idx {
    using value_type = pair<T, int>;
    using X = value_type;
    static constexpr bool is_small(const X& x, const X& y) {
        if (x.first < y.first) return true;
        if (x.first > y.first) return false;
        return (tie_is_left ? (x.second < y.second) : (x.second >= y.second));
    }
    static X op(X x, X y) { return (is_small(x, y) ? x : y); }
    static constexpr X unit() { return {numeric_limits<T>::max() / 2, -1}; }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MINIDX