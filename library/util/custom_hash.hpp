#ifndef AJAY_CUSTOM_HASH
#define AJAY_CUSTOM_HASH

namespace mitsuha{
namespace hashing {
    static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
 
#if USE_AES
    std::mt19937 rd(FIXED_RANDOM);
    const __m128i KEY1{(int64_t)rd(), (int64_t)rd()};
    const __m128i KEY2{(int64_t)rd(), (int64_t)rd()};
#endif
 
    template <class T, class D = void>
    struct custom_hash {};
 
    // https://www.boost.org/doc/libs/1_55_0/doc/html/hash/combine.html
    template <class T>
    inline void hash_combine(uint64_t& seed, const T& v) {
        custom_hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4);
    };
 
    // http://xorshift.di.unimi.it/splitmix64.c
    template <class T>
    struct custom_hash<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        uint64_t operator()(T _x) const {
            uint64_t x = _x;
#if USE_AES
            // implementation defined till C++17, defined from C++20
            __m128i m{int64_t(uint64_t(x) * 0xbf58476d1ce4e5b9uint64_t), (int64_t)FIXED_RANDOM};
            __m128i y = _mm_aesenc_si128(m, KEY1);
            __m128i z = _mm_aesenc_si128(y, KEY2);
            return z[0];
#else
            x += 0x9e3779b97f4a7c15 + FIXED_RANDOM;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
#endif
        }
    };
 
    template <class T>
    struct custom_hash<T, std::void_t<decltype(std::begin(std::declval<T>()))>> {
        uint64_t operator()(const T& a) const {
            uint64_t value = FIXED_RANDOM;
            for (auto& x : a) hash_combine(value, x);
            return value;
        }
    };
 
    template <class... T>
    struct custom_hash<std::tuple<T...>> {
        uint64_t operator()(const std::tuple<T...>& a) const {
            uint64_t value = FIXED_RANDOM;
            std::apply([&value](T const&... args) { (hash_combine(value, args), ...); }, a);
            return value;
        }
    };
 
    template <class T, class U>
    struct custom_hash<std::pair<T, U>> {
        uint64_t operator()(const std::pair<T, U>& a) const {
            uint64_t value = FIXED_RANDOM;
            hash_combine(value, a.first);
            hash_combine(value, a.second);
            return value;
        }
    };
 
}; // namespace hashing
}; // namespace mitsuha
#endif // AJAY_CUSTOM_HASH