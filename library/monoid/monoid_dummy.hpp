#ifndef AJAY_MONOID_DUMMY
#define AJAY_MONOID_DUMMY

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/alg/monoid/dummy.hpp
struct Monoid_Dummy {
    using value_type = char;
    static constexpr bool commute = true;
    static value_type op(value_type, value_type) { return 0; }
    static value_type unit() { return 0; }
};
} // namespace mitsuha
#endif // AJAY_MONOID_DUMMY
