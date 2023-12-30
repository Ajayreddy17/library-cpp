#ifndef AJAY_MONOID_SORTED
#define AJAY_MONOID_SORTED

namespace mitsuha{
template <class X>
struct Monoid_Sorted {
    struct Data{
        bool Sorted = true;
        bool Id = true;
        X L = 0;
        X R = 0;
    };
    using value_type = Data;
    static constexpr value_type op(const value_type &x, const value_type &y) noexcept {
        if (x.Id) return y;
        if (y.Id) return x;
        if (x.R <= y.L) return Data{bool(x.Sorted & y.Sorted), false, x.L, y.R};
        return Data{false, false, X(0), X(0)};
    }
    static constexpr value_type from_element(const X x) { return Data{true, false, x, x}; }
    static constexpr value_type unit() { return value_type{}; }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_SORTED
