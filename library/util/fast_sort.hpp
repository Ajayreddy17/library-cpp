#ifndef AJAY_FAST_SORT
#define AJAY_FAST_SORT

// fast_sort<Desc>(vec, proj)  : sort a vector by a key (default: the element itself).
//   * key = 8/16/32/64-bit integer, float, double, or std::pair of integers with
//     sizeof(first) + sizeof(second) <= 8 (lexicographic): LSD radix sort (stable) once n is
//     large enough for the passes needed, otherwise merge_sort on the packed key.
//   * any other key type: merge_sort.
//   Not stable in general (the radix path is, the merge path is not).
// merge_sort(vec, comp, proj) : 16-element sorting networks + branchless parity merges (not stable).
//                               Falls back to std::sort unless T is bit-copyable.
// fast_argsort<Desc>(vec)     : indices that sort vec.
//
// Radix details: early exit on already-sorted input; AND/OR of all keys restricts the
// passes to the bit range that actually varies; 11-bit digits (counters stay in L1);
// constant digits skipped; scatter unrolled x4; scratch buffer reused between calls
// (static buffers: not thread-safe, and the projection must not call fast_sort itself).

namespace mitsuha {
namespace fast_sort_internal {

struct Identity {
    template <class T> constexpr T&& operator()(T&& x) const noexcept { return std::forward<T>(x); }
};

template <int Bytes> struct UInt;
template <> struct UInt<1> { using type = uint8_t; };
template <> struct UInt<2> { using type = uint16_t; };
template <> struct UInt<4> { using type = uint32_t; };
template <> struct UInt<8> { using type = uint64_t; };

template <class K, class = void> struct KeyTraits { static constexpr bool ok = false; };
template <class K>
struct KeyTraits<K, std::enable_if_t<std::is_integral<K>::value && sizeof(K) <= 8>> {
    static constexpr bool ok = true;
    using U = typename UInt<sizeof(K)>::type;
    static constexpr int bits = sizeof(K) * 8;
    static U conv(K x) {
        U y = static_cast<U>(x);
        if constexpr (std::is_signed<K>::value) y ^= U(1) << (bits - 1);
        return y;
    }
};
template <class K>
struct KeyTraits<K, std::enable_if_t<std::is_same<K, float>::value || std::is_same<K, double>::value>> {
    static constexpr bool ok = true;
    using U = typename UInt<sizeof(K)>::type;
    static constexpr int bits = sizeof(K) * 8;
    static U conv(K x) {
        U y;
        std::memcpy(&y, &x, sizeof(K));
        return y ^ ((y >> (bits - 1)) ? ~U(0) : (U(1) << (bits - 1)));
    }
};

// std::pair of integers packed into one key (first in the high bits): lexicographic order.
template <class A, class B>
struct KeyTraits<std::pair<A, B>, std::enable_if_t<std::is_integral<A>::value && std::is_integral<B>::value &&
                                                  sizeof(A) + sizeof(B) <= 8>> {
    static constexpr bool ok = true;
    using U = std::conditional_t<(sizeof(A) + sizeof(B) <= 4), uint32_t, uint64_t>;
    static constexpr int bits = (sizeof(A) + sizeof(B)) * 8;
    static U conv(const std::pair<A, B>& x) {
        return U(KeyTraits<A>::conv(x.first)) << (8 * sizeof(B)) | U(KeyTraits<B>::conv(x.second));
    }
};

// "Plain" element: can be bit-copied (true for scalars, PODs, std::pair / std::tuple of scalars).
template <class T>
constexpr bool is_plain = std::is_trivially_copy_constructible<T>::value && std::is_trivially_destructible<T>::value;

// Reused scratch memory (no zero-initialisation, no reallocation between calls).
template <class T> T* scratch(size_t n) {
    static std::vector<unsigned char> buf;
    if (buf.size() < n * sizeof(T)) buf.resize(n * sizeof(T));
    return reinterpret_cast<T*>(buf.data());
}

// ---- radix sort -------------------------------------------------------------------------
// Returns false (array untouched) when n < P * min_per_pass, P = number of passes needed:
// then a comparison sort is cheaper than clearing / prefix-summing P * 2048 counters.
template <class U, class T, class KeyF>
bool radix_core(T* p, uint32_t n, KeyF key, uint32_t min_per_pass) {
    static_assert(is_plain<T>, "radix_core needs a plain (bit-copyable) T");
    constexpr int B = 11;  // 2048 counters per pass stay in L1 (8- and 16-bit digits measured slower)
    constexpr uint32_t R = 1u << B, M = R - 1;
    constexpr int PMAX = (sizeof(U) * 8 + B - 1) / B;
    {  // already sorted? (stops at the first descent, so ~free on unsorted input)
        uint32_t i = 1;
        while (i < n && !(key(p[i]) < key(p[i - 1]))) ++i;
        if (i == n) return true;
    }
    U o = 0, a = ~U(0);
    for (uint32_t i = 0; i != n; ++i) {
        U k = key(p[i]);
        o |= k, a &= k;
    }
    const uint64_t vary = uint64_t(o ^ a);  // bits that differ between keys
    const int lo = __builtin_ctzll(vary), hi = 63 - __builtin_clzll(vary);
    const int P = (hi - lo + B) / B;
    if (n < P * min_per_pass) return false;
    static uint32_t cnt[PMAX][R];
    std::memset(cnt, 0, sizeof(uint32_t) * R * P);
    {
        uint32_t i = 0;
        for (; i + 4 <= n; i += 4) {
            U k0 = key(p[i]) >> lo, k1 = key(p[i + 1]) >> lo, k2 = key(p[i + 2]) >> lo, k3 = key(p[i + 3]) >> lo;
            for (int q = 0; q < P; ++q) {
                const int s = B * q;
                ++cnt[q][(k0 >> s) & M], ++cnt[q][(k1 >> s) & M], ++cnt[q][(k2 >> s) & M], ++cnt[q][(k3 >> s) & M];
            }
        }
        for (; i != n; ++i) {
            U k = key(p[i]) >> lo;
            for (int q = 0; q < P; ++q) ++cnt[q][(k >> (B * q)) & M];
        }
    }
    T *from = p, *to = scratch<T>(n);
    for (int q = 0; q < P; ++q) {
        uint32_t* c = cnt[q];
        const int s = lo + B * q;
        if (c[(key(from[0]) >> s) & M] == n) continue;  // constant digit
        uint32_t sum = 0;
        for (uint32_t i = 0; i != R; ++i) { uint32_t t = c[i]; c[i] = sum; sum += t; }
        uint32_t i = 0;
        for (; i + 4 <= n; i += 4) {
            __builtin_prefetch(from + i + 64);
            uint32_t d0 = (key(from[i]) >> s) & M, d1 = (key(from[i + 1]) >> s) & M;
            uint32_t d2 = (key(from[i + 2]) >> s) & M, d3 = (key(from[i + 3]) >> s) & M;
            to[c[d0]++] = from[i], to[c[d1]++] = from[i + 1], to[c[d2]++] = from[i + 2], to[c[d3]++] = from[i + 3];
        }
        for (; i != n; ++i) to[c[(key(from[i]) >> s) & M]++] = from[i];
        std::swap(from, to);
    }
    if (from != p) std::memcpy(static_cast<void*>(p), from, sizeof(T) * n);
    return true;
}

// ---- comparison sort: networks on blocks of 16 + branchless parity merges -----------------
constexpr uint32_t NET16[60][2] = {
    {0,13},{1,12},{2,15},{3,14},{4,8},{5,6},{7,11},{9,10},
    {0,5},{1,7},{2,9},{3,4},{6,13},{8,14},{10,15},{11,12},
    {0,1},{2,3},{4,5},{6,8},{7,9},{10,11},{12,13},{14,15},
    {0,2},{1,3},{4,10},{5,11},{6,7},{8,9},{12,14},{13,15},
    {1,2},{3,12},{4,6},{5,7},{8,10},{9,11},{13,14},
    {1,4},{2,6},{5,8},{7,10},{9,13},{11,14},
    {2,4},{3,6},{9,12},{11,13},
    {3,5},{6,8},{7,9},{10,12},
    {3,4},{5,6},{7,8},{9,10},{11,12},
    {6,7},{8,9}};

// Branch-free "dst = f ? *b : *a" for plain T. For sizes 1/2/4/8 select on the raw bits
// (cmov / csel): std::pair's user-provided operator= otherwise compiles to branches (4x slower).
template <class T> inline void sel(T* dst, bool f, const T* a, const T* b) {
    if constexpr (sizeof(T) == 8 || sizeof(T) == 4 || sizeof(T) == 2 || sizeof(T) == 1) {
        typename UInt<sizeof(T)>::type x, y;
        std::memcpy(&x, a, sizeof(T)), std::memcpy(&y, b, sizeof(T));
        x = f ? y : x;
        std::memcpy(static_cast<void*>(dst), &x, sizeof(T));
    } else {  // larger: select the pointer (one cmov), then copy (2 cmovs for 16B measured slower)
        std::memcpy(static_cast<void*>(dst), f ? b : a, sizeof(T));
    }
}

template <class T, class Less>
inline void sort16(T* p, Less& lt) {
    for (int i = 0; i < 60; ++i) {
        const uint32_t a = NET16[i][0], b = NET16[i][1];
        const bool f = lt(p[b], p[a]);
        alignas(T) unsigned char ta[sizeof(T)], tb[sizeof(T)];
        std::memcpy(ta, p + a, sizeof(T)), std::memcpy(tb, p + b, sizeof(T));
        sel(p + a, f, (const T*)ta, (const T*)tb);
        sel(p + b, f, (const T*)tb, (const T*)ta);
    }
}

// Merge two sorted runs of equal length h from both ends at once (quadsort's parity merge):
// no bounds checks are needed and the two halves are independent dependency chains.
template <class T, class Less>
inline void parity_merge(const T* l, const T* r, uint32_t h, T* d, Less& lt) {
    const T *pl = l, *pr = r, *tl = l + h - 1, *tr = r + h - 1;
    T *pd = d, *td = d + 2 * h - 1;
    for (uint32_t k = 0; k != h; ++k) {
        const bool x = lt(*pr, *pl);
        sel(pd++, x, pl, pr), pr += x, pl += !x;
        const bool y = lt(*tr, *tl);
        sel(td--, y, tr, tl), tl -= y, tr -= !y;
    }
}
// Two independent parity merges interleaved: 4 dependency chains in flight.
template <class T, class Less>
inline void parity_merge_x2(const T* l, uint32_t h, T* d, Less& lt) {
    const T *pl = l, *pr = l + h, *tl = l + h - 1, *tr = l + 2 * h - 1;
    const T *ql = l + 2 * h, *qr = l + 3 * h, *sl = l + 3 * h - 1, *sr = l + 4 * h - 1;
    T *pd = d, *td = d + 2 * h - 1, *qd = d + 2 * h, *sd = d + 4 * h - 1;
    for (uint32_t k = 0; k != h; ++k) {
        const bool x = lt(*pr, *pl);
        sel(pd++, x, pl, pr), pr += x, pl += !x;
        const bool y = lt(*tr, *tl);
        sel(td--, y, tr, tl), tl -= y, tr -= !y;
        const bool x2 = lt(*qr, *ql);
        sel(qd++, x2, ql, qr), qr += x2, ql += !x2;
        const bool y2 = lt(*sr, *sl);
        sel(sd--, y2, sr, sl), sl -= y2, sr -= !y2;
    }
}
// Unequal runs: merge from both ends in batches of k = min(la, lb) / 2 steps per end.
// Within a batch at most 2k <= min(la, lb) elements leave each run, so no bounds checks.
template <class T, class Less>
inline void merge_runs(const T* a, uint32_t na, const T* b, uint32_t nb, T* d, Less& lt) {
    const T *pl = a, *pr = b, *el = a + na, *er = b + nb;  // remaining: [pl, el), [pr, er)
    T *pd = d, *td = d + na + nb;
    while (true) {
        const uint32_t k = uint32_t(std::min(el - pl, er - pr)) / 2;
        if (k == 0) break;
        for (uint32_t t = 0; t != k; ++t) {
            const bool x = lt(*pr, *pl);
            sel(pd++, x, pl, pr), pr += x, pl += !x;
            const bool y = lt(er[-1], el[-1]);
            sel(--td, y, er - 1, el - 1), el -= y, er -= !y;
        }
    }
    while (pl != el && pr != er) {
        const bool x = lt(*pr, *pl);
        sel(pd++, x, pl, pr), pr += x, pl += !x;
    }
    while (pl != el) *pd++ = *pl++;
    while (pr != er) *pd++ = *pr++;
}

template <class T, class Less>
void merge_sort_impl(T* p, uint32_t n, Less lt) {
    constexpr uint32_t W = 16;
    const uint32_t full = n / W * W;
    for (uint32_t i = 0; i != full; i += W) sort16(p + i, lt);
    for (uint32_t i = full + 1; i < n; ++i) {  // tail: insertion sort
        T t = p[i];
        uint32_t j = i;
        for (; j != full && lt(t, p[j - 1]); --j) p[j] = p[j - 1];
        p[j] = t;
    }
    if (n <= W) return;
    T *from = p, *to = scratch<T>(n);
    for (uint32_t w = W; w < n; w *= 2) {
        uint32_t i = 0;
        for (; i + 4 * w <= n; i += 4 * w) parity_merge_x2(from + i, w, to + i, lt);
        for (; i + 2 * w <= n; i += 2 * w) parity_merge(from + i, from + i + w, w, to + i, lt);
        if (i + w < n) merge_runs(from + i, w, from + i + w, n - i - w, to + i, lt);
        else std::memcpy(static_cast<void*>(to + i), from + i, sizeof(T) * (n - i));
        std::swap(from, to);
    }
    if (from != p) std::memcpy(static_cast<void*>(p), from, sizeof(T) * n);
}
}  // namespace fast_sort_internal

template <class T, class Comp = std::less<>, class Proj = fast_sort_internal::Identity>
void merge_sort(std::vector<T>& a, Comp comp = {}, Proj proj = {}) {
    auto lt = [&](const T& x, const T& y) { return comp(proj(x), proj(y)); };
    if constexpr (fast_sort_internal::is_plain<T>) {
        if (a.size() < 2) return;
        fast_sort_internal::merge_sort_impl(a.data(), (uint32_t)a.size(), lt);
    } else {
        std::sort(a.begin(), a.end(), lt);
    }
}

template <bool Desc = false, class T, class Proj = fast_sort_internal::Identity>
void fast_sort(std::vector<T>& a, Proj proj = {}) {
    using namespace fast_sort_internal;
    const uint32_t n = a.size();
    if (n < 2) return;
    using K = std::decay_t<decltype(proj(a[0]))>;
    using KT = KeyTraits<K>;
    if constexpr (KT::ok && is_plain<T>) {
        using U = typename KT::U;
        auto key = [&](const T& x) -> U {
            U k = KT::conv(proj(x));
            return Desc ? U(~k) : k;
        };
        auto lt = [&](const T& x, const T& y) { return key(x) < key(y); };
        // radix needs n >= passes * min_per_pass; below that a comparison sort is cheaper (measured)
        constexpr uint32_t min_per_pass = sizeof(T) <= 8 ? 160 : 64;
        if (n >= 64 && radix_core<U>(a.data(), n, key, min_per_pass)) return;
        if (sizeof(T) > 8 && n < 192) std::sort(a.begin(), a.end(), lt);  // big elements: fewer moves
        else merge_sort_impl(a.data(), n, lt);
    } else {
        if constexpr (Desc) merge_sort(a, std::greater<>(), proj);
        else merge_sort(a, std::less<>(), proj);
    }
}

template <bool Desc = false, class T>
std::vector<int> fast_argsort(const std::vector<T>& a) {
    std::vector<int> ids(a.size());
    std::iota(ids.begin(), ids.end(), 0);
    fast_sort<Desc>(ids, [&](int i) -> const T& { return a[i]; });
    return ids;
}
}  // namespace mitsuha
#endif  // AJAY_FAST_SORT
