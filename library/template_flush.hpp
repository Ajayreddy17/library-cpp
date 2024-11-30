#ifndef LOCAL
#pragma GCC optimize("Ofast", "unroll-loops")
#endif

#include <bits/stdc++.h>
#include "library/io/flush.hpp"

namespace mitsuha {
template <class T> bool chmin(T& x, const T& y) { 
    return y >= x ? false : (x = y, true); 
}
template <class T> bool chmax(T& x, const T& y) { 
    return y <= x ? false : (x = y, true); 
}
template <class T> constexpr T fld(const T x, const T y) { 
    T q = x / y, r = x % y; return q - ((x ^ y) < 0 and (r != 0)); 
}
template <class T> constexpr T cld(const T x, const T y) { 
    T q = x / y, r = x % y; return q + ((x ^ y) > 0 and (r != 0)); 
}
template <class T> constexpr T rem(const T x, const T y) { 
    return x - y * fld(x, y); 
}
template <class Iterable> void settify(Iterable& a) { 
    std::sort(a.begin(), a.end()), a.erase(std::unique(a.begin(), a.end()), a.end()); 
}
template <typename T, typename... Vectors>
void concat(std::vector<T> &first, const Vectors &... others) {
    std::vector<T> &res = first;
    (res.insert(res.end(), others.begin(), others.end()), ...);
}
template<typename T>
std::map<T, int> Counter(std::vector<T> &a){
    std::map<T, int> cnt;
    for (auto &x: a) ++cnt[x];
    return cnt;
}
template <typename T>
std::vector<int> argsort(const std::vector<T> &A) {
    std::vector<int> ids(A.size());
    std::iota(ids.begin(), ids.end(), 0);
    std::sort(ids.begin(), ids.end(), [&](int i, int j) { 
        return (A[i] == A[j] ? i < j : A[i] < A[j]); 
    });
    return ids;
}
template <typename T>
std::vector<T> rearrange(const std::vector<T> &A, const std::vector<int> &I) {
    std::vector<T> B(I.size());
    for(int i = 0; i < I.size(); ++i) B[i] = A[I[i]];
    return B;
}
template <size_t D> struct Dim : std::array<int, D> {
    template <typename ...Ints> Dim(const Ints& ...ns) : 
        std::array<int, D>::array{ static_cast<int>(ns)... } {}
};
template <typename ...Ints> Dim(const Ints& ...) -> Dim<sizeof...(Ints)>;
template <class T, size_t D, size_t I = 0>
auto ndvec(const Dim<D> &ns, const T& value = {}) {
    if constexpr (I + 1 < D) {
        return std::vector(ns[I], ndvec<T, D, I + 1>(ns, value));
    } else {
        return std::vector<T>(ns[I], value);
    }
}
}

namespace mitsuha {
using str = std::string;
using int128 = __int128;
using uint128 = unsigned __int128;
template <class T> using min_priority_queue 
                            = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <class T> using max_priority_queue 
                            = std::priority_queue<T, std::vector<T>, std::less<T>>;
}
namespace mitsuha { 
    const std::vector<std::string> Yes = {"No", "Yes"};
    const std::vector<std::string> YES = {"NO", "YES"};
}
 
#ifndef __COUNTER__
#define __COUNTER__ __LINE__
#endif

#define TL (long long)

#define OVERLOAD5(a, b, c, d, e, ...) e
#define REP1_0(b, c) REP1_1(b, c)
#define REP1_1(b, c) for (long long REP_COUNTER_##c = 0; REP_COUNTER_##c < TL(b); ++REP_COUNTER_##c)
#define REP1(b) REP1_0(b, __COUNTER__)
#define REP2(i, b) for (long long i = 0; i < TL(b); ++i)
#define REP3(i, a, b) for (long long i = TL(a); i < TL(b); ++i)
#define REP4(i, a, b, c) for (long long i = TL(a); i < TL(b); i += TL(c))
#define For(...) OVERLOAD5(__VA_ARGS__, REP4, REP3, REP2, REP1)(__VA_ARGS__)
#define RREP2(i, a) for (long long i = TL(a)-1; i >= 0; --i)
#define RREP3(i, a, b) for (long long i = TL(b)-1; i >= TL(a); --i)
#define RREP4(i, a, b, c) for (long long i = TL(b)-1; i >= TL(a); i -= TL(c))
#define Frr(...) OVERLOAD5(__VA_ARGS__, RREP4, RREP3, RREP2)(__VA_ARGS__)

#define All(iterable) std::begin(iterable), std::end(iterable)
#define len(iterable) TL iterable.size()
#define elif else if

#define KBIT(a, k) ((a >> k) & 1)

using namespace mitsuha;
using namespace std;

#ifdef LOCAL
#define  debug_path "library/debug/pprint.hpp"
#include debug_path
#define Assert(x) assert(x)
#else
#define debug(...) void(0)
#define debug2(...) void(0)
#define debugbin(...) void(0)
#define Assert(x) void(0)
#endif
 
constexpr int iinf = std::numeric_limits<int>::max() / 2;
constexpr long long linf = std::numeric_limits<long long>::max() / 2;