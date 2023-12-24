#ifndef AJAY_MONOID_MINMINCNT
#define AJAY_MONOID_MINMINCNT

namespace mitsuha{
template <typename E>
struct Monoid_MinMincnt {
    using value_type = pair<E, E>;
    using X = value_type;
    static X op(X x, X y) {
        auto [xmin, xmincnt] = x;
        auto [ymin, ymincnt] = y;
        if (xmin > ymin) return y;
        if (xmin < ymin) return x;
        return {xmin, xmincnt + ymincnt};
    }
    static constexpr X unit() { return {numeric_limits<E>::max() / 2, 0}; }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MINMINCNT
