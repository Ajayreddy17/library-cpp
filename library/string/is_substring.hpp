#ifndef AJAY_IS_SUBSTRING
#define AJAY_IS_SUBSTRING

#include "library/string/zalgorithm.hpp"

namespace mitsuha{
// Whether to include it in continuous subsequences. Linear time with z-algo
template <typename STRING>
bool is_substring(STRING& S, STRING& T) {
    int n = int(S.size()), m = int(T.size());
    STRING ST;
    for (auto&& x: S) ST.push_back(x);
    for (auto&& x: T) ST.push_back(x);
    auto Z = zalgorithm(ST);
    for (int i = n; i < n + m; ++i) {
        if (Z[i] >= n) return true;
    }
    return false;
}
} // namespace mitsuha
#endif // AJAY_IS_SUBSTRING