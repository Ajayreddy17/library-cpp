#ifndef AJAY_MONOID_ROLLINGHASH
#define AJAY_MONOID_ROLLINGHASH

#include "library/random/base.hpp"
#include "library/mod/modint61.hpp"

namespace mitsuha{
struct Monoid_Rolling_Hash {
    using value_type = pair<modint61, modint61>;
    using X = value_type;

    static unsigned long long &get_param() {
        static unsigned long long base = 0;
        return base;
    }
    static void set_param(unsigned long long base) { get_param() = base; }

    static X from_element(unsigned long long x) {
        while (get_param() == 0) set_param(RNG_64());
        return {get_param(), x};
    }
    static X op(X x, X y) { return {x.first * y.first, x.second * y.first + y.second}; }
    static constexpr X unit() { return {1, 0}; }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ROLLINGHASH