#ifndef AJAY_USE_CACHE
#define AJAY_USE_CACHE

#include "library/util/custom_hash.hpp"
#include "library/datastructure/pbds.hpp"

namespace mitsuha{
template <typename Signature, typename Lambda>
struct Cache;
 
template <typename ReturnType, typename... Args, typename Lambda>
struct Cache<ReturnType(Args...), Lambda> {
    template <typename... DummyArgs>
    ReturnType operator()(DummyArgs&&... args) {
        auto tied_args = std::tie(args...);
        auto it = memo.find(tied_args);
        if (it == memo.end()) {
            auto&& ans = f(*this, std::forward<DummyArgs>(args)...);
            memo[tied_args] = ans;
            return ans;
        } else {
            return it->second;
        }
    }
 
    template <class _Lambda>
    Cache(std::tuple<>, _Lambda&& _f) : f(std::forward<_Lambda>(_f)) {}
 
    Lambda f;
    using TiedArgs = std::tuple<std::decay_t<Args>...>;
    pbds::unordered_map<TiedArgs, ReturnType, hashing::custom_hash<TiedArgs>> memo;
};
 
template <class Signature, class Lambda>
auto use_cache(Lambda&& f) {
    return Cache<Signature, Lambda>(std::tuple{}, std::forward<Lambda>(f));
}
} // namespace mitsuha
#endif // AJAY_USE_CACHE