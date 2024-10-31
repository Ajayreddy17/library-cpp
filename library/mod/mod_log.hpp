#ifndef AJAY_MOD_LOG
#define AJAY_MOD_LOG

#include "library/mod/dynamic_modint.hpp"
#include "library/number/discrete_log.hpp"
#include "library/monoid/monoid_mul.hpp"

namespace mitsuha{
int mod_log(int mod, long long a, long long b) {
    dmint::set_mod(mod);
    return discrete_log_monoid<Monoid_Mul<dmint>>(
        dmint(a), dmint(b), [](auto x) { return x.val; }, 0, mod);
}
} // namespace mitsuha
#endif // AJAY_MOD_LOG