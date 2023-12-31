#ifndef AJAY_UPDATE_PROXY_OBJECT
#define AJAY_UPDATE_PROXY_OBJECT

#include "library/type_traits/constraints.hpp"

namespace mitsuha{
template <typename T, typename UpdateFunc, constraints_t<std::is_invocable<UpdateFunc>> = nullptr>
struct UpdateProxyObject {
public:
    UpdateProxyObject(T &v, UpdateFunc update) : v(v), update(update) {}
    operator T() const { return v; }
    auto& operator++() && { ++v, update(); return *this; }
    auto& operator--() && { --v, update(); return *this; }
    auto& operator+=(const T &val) && { v += val, update(); return *this; }
    auto& operator-=(const T &val) && { v -= val, update(); return *this; }
    auto& operator*=(const T &val) && { v *= val, update(); return *this; }
    auto& operator/=(const T &val) && { v /= val, update(); return *this; }
    auto& operator%=(const T &val) && { v %= val, update(); return *this; }
    auto& operator =(const T &val) && { v  = val, update(); return *this; }
    auto& operator<<=(const T &val) && { v <<= val, update(); return *this; }
    auto& operator>>=(const T &val) && { v >>= val, update(); return *this; }
    template <typename F, constraints_t<is_same_as_invoke_result<T, F, T>> = nullptr>
    auto& apply(F f) && { v = f(v), update(); return *this; }
private:
    T &v;       
    UpdateFunc update;
};
} // namespace mitsuha
#endif // AJAY_UPDATE_PROXY_OBJECT