#ifndef AJAY_MONOID_MAXIDX
#define AJAY_MONOID_MAXIDX

namespace mitsuha{
template <typename T, bool tie_is_left = true>
struct Monoid_Max_Idx {
    using value_type = pair<T, int>;
    using X = value_type;
    static X op(X x, X y) {
        if (x.first > y.first) return x;
        if (x.first < y.first) return y;
        if (x.second > y.second) swap(x, y);
        return (tie_is_left ? x : y);
    }
    static constexpr X unit() { return {numeric_limits<T>::min() / 2, -1}; }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MAXIDX