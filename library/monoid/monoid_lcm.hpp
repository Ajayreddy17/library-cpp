#ifndef AJAY_MONOID_LCM
#define AJAY_MONOID_LCM

namespace mitsuha{
// numeric_limits<T>::max() / 2 when large
template <typename T>
struct Monoid_Lcm {
    using value_type = T;
    using X = value_type;
    static X op(X x, X y) {
        if (x == numeric_limits<T>::max() / 2 or y == numeric_limits<T>::max() / 2) {
            return numeric_limits<T>::max() / 2;
        }
        x /= gcd(x, y);
        if (x >= ceil(numeric_limits<T>::max() / 2, y)) {
            return numeric_limits<T>::max() / 2;
        }
        return x * y;
    }
    static constexpr X unit() { return 1; }
    static constexpr bool commute = 1;
};
} // namespace mitsuha
#endif // AJAY_MONOID_LCM
