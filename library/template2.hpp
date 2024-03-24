#ifndef LOCAL
#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("avx2", "popcnt")
#endif

#include <bits/stdc++.h>
namespace mitsuha {
template <class T> bool chmin(T& x, const T& y) { return y >= x ? false : (x = y, true); }
template <class T> bool chmax(T& x, const T& y) { return y <= x ? false : (x = y, true); }
template <class T> constexpr T floor(const T x, const T y) { T q = x / y, r = x % y; return q - ((x ^ y) < 0 and (r != 0)); }
template <class T> constexpr T ceil(const T x, const T y) { T q = x / y, r = x % y; return q + ((x ^ y) > 0 and (r != 0)); }
template <class T> constexpr T bmod(const T x, const T y) { return x - y * floor(x, y); }
}

namespace mitsuha::macro {
#define IMPL_REPITER(cond) auto& begin() { return *this; } auto end() { return nullptr; } auto& operator*() { return _val; } auto& operator++() { return _val += _step, *this; } bool operator!=(std::nullptr_t) { return cond; }
template <class Int, class IntL = Int, class IntStep = Int, std::enable_if_t<(std::is_signed_v<Int> == std::is_signed_v<IntL>), std::nullptr_t> = nullptr> struct rep_impl {
    Int _val; const Int _end, _step;
    rep_impl(Int n) : rep_impl(0, n) {}
    rep_impl(IntL l, Int r, IntStep step = 1) : _val(l), _end(r), _step(step) {}
    IMPL_REPITER((_val < _end))
};
template <class Int, class IntL = Int, class IntStep = Int, std::enable_if_t<(std::is_signed_v<Int> == std::is_signed_v<IntL>), std::nullptr_t> = nullptr> struct rrep_impl {
    Int _val; const Int _end, _step;
    rrep_impl(Int n) : rrep_impl(0, n) {}
    rrep_impl(IntL l, Int r) : _val(r - 1), _end(l), _step(-1) {}
    rrep_impl(IntL l, Int r, IntStep step) : _val(l + floor<Int>(r - l - 1, step) * step), _end(l), _step(-step) {}
    IMPL_REPITER((_val >= _end))
};
#undef IMPL_REPITER
}

#include <unistd.h>
namespace mitsuha::io {
static constexpr uint32_t SZ = 1 << 17;
char ibuf[SZ], obuf[SZ], out[100];
uint32_t pil = 0, pir = 0, por = 0;
struct Pre {
    char num[10000][4];
    constexpr Pre() : num() {
        for (int i = 0; i < 10000; i++) {
            int n = i;
            for (int j = 3; j >= 0; j--) { num[i][j] = n % 10 | '0'; n /= 10; }
        }
    }
} constexpr pre;

inline void load() {
    memcpy(ibuf, ibuf + pil, pir - pil);
    pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
    pil = 0;
}
inline void flush() { fwrite(obuf, 1, por, stdout); por = 0; }

void rd(char &c) { do { if (pil + 1 > pir) load(); c = ibuf[pil++]; } while (isspace(c)); }
void rd(std::string &x) {
    x.clear(); char c;
    do { if (pil + 1 > pir) load(); c = ibuf[pil++]; } while (isspace(c));
    do { x += c; if (pil == pir) load(); if (pil == pir) break; c = ibuf[pil++]; } while (!isspace(c));
}
template <typename T> void rd_real(T &x) { std::string s; rd(s); x = stod(s); }
template <typename T>
void rd_integer(T &x) {
    if (pil + 100 > pir) load();
    char c;
    do { c = ibuf[pil++]; }while (c < '-');
    bool minus = 0;
    if constexpr (std::is_signed<T>::value || std::is_same_v<T, __int128>) { if (c == '-') { minus = 1, c = ibuf[pil++]; } }
    x = 0;
    while (c >= '0') { x = x * 10 + (c & 15), c = ibuf[pil++]; }
    if constexpr (std::is_signed<T>::value || std::is_same_v<T, __int128>) { if (minus) x = -x; }
}

template<typename T> std::enable_if_t<std::is_integral_v<T> || 
            std::is_floating_point_v<T>, void>
rd(T &x) {
    if constexpr (std::is_integral_v<T>) {
        rd_integer(x);
    } else {
        rd_real(x);
    }
}
    
template <class T, class U> 
void rd(std::pair<T, U> &p) { 
    return rd(p.first), rd(p.second); 
}
template <size_t N = 0, typename T>
void rd(T &t) { if constexpr (N < std::tuple_size<T>::value) { auto &x = std::get<N>(t); rd(x); rd<N + 1>(t); } }
template <class... T> void rd(std::tuple<T...> &tpl) { rd(tpl); }
template <size_t N = 0, typename T> void rd(std::array<T, N> &x) { for (auto &d: x) rd(d); }
template <class T> void rd(std::vector<T> &x) { for (auto &d: x) rd(d); }

void read() {}
template <class H, class... T> void read(H &h, T &... t) { rd(h), read(t...); }

void wt(const char c) { if (por == SZ) flush(); obuf[por++] = c; }
void wt(const std::string &s) { for (char c: s) wt(c); }
void wt(const char *s) { size_t len = strlen(s); for (size_t i = 0; i < len; i++) wt(s[i]); }
template <typename T> void wt_integer(T x) {
    if (por > SZ - 100) flush();
    if (x < 0) { obuf[por++] = '-', x = -x; }
    int outi;
    for (outi = 96; x >= 10000; outi -= 4) { memcpy(out + outi, pre.num[x % 10000], 4); x /= 10000; }
    if (x >= 1000) { memcpy(obuf + por, pre.num[x], 4); por += 4; } 
    else if (x >= 100) { memcpy(obuf + por, pre.num[x] + 1, 3); por += 3; } 
    else if (x >= 10) { int q = (x * 103) >> 10; obuf[por] = q | '0'; obuf[por + 1] = (x - q * 10) | '0'; por += 2; } 
    else obuf[por++] = x | '0';
    memcpy(obuf + por, out + outi + 4, 96 - outi); por += 96 - outi;
}
template <typename T>
void wt_real(T x) {
    std::ostringstream oss; oss << std::fixed << std::setprecision(15) << double(x);
    std::string s = oss.str(); wt(s);
}
void wt(int x) { wt_integer(x); }
void wt(long int x) { wt_integer(x); }
void wt(long long x) { wt_integer(x); }
void wt(__int128 x) { wt_integer(x); }
void wt(unsigned int x) { wt_integer(x); }
void wt(unsigned long long x) { wt_integer(x); }
void wt(unsigned __int128 x) { wt_integer(x); }
void wt(double x) { wt_real(x); }
void wt(long double x) { wt_real(x); }
void wt(__float128 x) { wt_real(x); }
    
template <class T, class U> void wt(const std::pair<T, U> val) { wt(val.first); wt(' '); wt(val.second); }
template <size_t N = 0, typename T> void wt_tuple(const T t) {
    if constexpr (N < std::tuple_size<T>::value) {
        if constexpr (N > 0) { wt(' '); }
        const auto x = std::get<N>(t);
        wt(x); wt_tuple<N + 1>(t);
    }
}
template <class... T> void wt(std::tuple<T...> tpl) { wt_tuple(tpl); }
template <class T, size_t S> void wt(const std::array<T, S> val) {
    auto n = val.size(); for (size_t i = 0; i < n; i++) { if (i) wt(' '); wt(val[i]); }
}
template <class T> void wt(const std::vector<T> val) {
    auto n = val.size(); for (size_t i = 0; i < n; i++) { if (i) wt(' '); wt(val[i]); }
}

void print() { wt('\n'); }
template <class Head, class... Tail>
void print(Head &&head, Tail &&... tail) { wt(head); if (sizeof...(Tail)) wt(' '); print(std::forward<Tail>(tail)...); }
void __attribute__((destructor)) _d() { flush(); }
} // namespace mitsuha::io
namespace mitsuha{ using io::read; using io::print; using io::flush; }

namespace mitsuha {
template <class T, class ToKey, class CompKey = std::less<>, std::enable_if_t<std::conjunction_v<std::is_invocable<ToKey, T>, std::is_invocable_r<bool, CompKey, std::invoke_result_t<ToKey, T>, std::invoke_result_t<ToKey, T>>>, std::nullptr_t> = nullptr>
auto lambda(const ToKey& to_key, const CompKey& comp_key = std::less<>()) {
    return [=](const T& x, const T& y) { return comp_key(to_key(x), to_key(y)); };
}
template <class Compare, std::enable_if_t<std::is_invocable_r_v<bool, Compare, int, int>, std::nullptr_t> = nullptr>
std::vector<int> sorted_indices(int n, const Compare& compare) {
    std::vector<int> p(n);
    return std::iota(p.begin(), p.end(), 0), std::sort(p.begin(), p.end(), compare), p;
}
template <class ToKey, std::enable_if_t<std::is_invocable_v<ToKey, int>, std::nullptr_t> = nullptr>
std::vector<int> sorted_indices(int n, const ToKey& to_key) { return sorted_indices(n, lambda<int>(to_key)); }

template <typename T, typename Gen>
auto generate_vector(int n, Gen generator) { std::vector<T> v(n); for (int i = 0; i < n; ++i) v[i] = generator(i); return v; }
template <typename T> auto generate_range(T l, T r) { return generate_vector<T>(r - l, [l](int i) { return l + i; }); }
template <typename T> auto generate_range(T n) { return generate_range<T>(0, n); }

template <class Iterable>
void settify(Iterable& a) { std::sort(a.begin(), a.end()), a.erase(std::unique(a.begin(), a.end()), a.end()); }

template <size_t D> struct Dim : std::array<int, D> {
    template <typename ...Ints> Dim(const Ints& ...ns) : std::array<int, D>::array{ static_cast<int>(ns)... } {}
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
} // namescape mitsuha

namespace mitsuha {
using str = std::string;
using int128 = __int128;
using uint128 = unsigned __int128;
template <class T> using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <class T> using max_priority_queue = std::priority_queue<T, std::vector<T>, std::less<T>>;
}
namespace mitsuha { const std::string Yes = "Yes", No = "No", YES = "YES", NO = "NO"; }

#define Int(...) int __VA_ARGS__; read(__VA_ARGS__)
#define Ll(...) long long __VA_ARGS__; read(__VA_ARGS__)
#define Dbl(...) double __VA_ARGS__; read(__VA_ARGS__)
#define Chr(...) char __VA_ARGS__; read(__VA_ARGS__)
#define Str(...) string __VA_ARGS__; read(__VA_ARGS__)
#define Vt(type, name, size) vector<type> name(size); read(name)
#define Vvt(type, name, h, w) vector<vector<type>> name(h, vector<type>(w)); read(name)
#define die(...)  do { print(__VA_ARGS__); return; } while (false)
#define kill(...) do { print(__VA_ARGS__); return 0; } while (false)

#define Each(e, v) for (auto &&e : v)
#define CFor(e, v) for (const auto &e : v)
#define For(i, ...) CFor(i, mitsuha::macro::rep_impl(__VA_ARGS__))
#define Frr(i, ...) CFor(i, mitsuha::macro::rrep_impl(__VA_ARGS__))
#define Loop(n) for ([[maybe_unused]] const auto& _ : mitsuha::macro::rep_impl(n))

#define All(iterable) std::begin(iterable), std::end(iterable)
#define len(iterable) (long long) iterable.size()
#define elif else if
 
using namespace mitsuha;
using namespace std;

#ifdef LOCAL
/*-*/#include "library/debug/pprint.hpp"
#else
#define debug(...) void(0)
#endif
 
constexpr int iinf = std::numeric_limits<int>::max() / 2;
constexpr long long linf = std::numeric_limits<long long>::max() / 2;