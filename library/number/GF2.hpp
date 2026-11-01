#ifndef AJAY_GF2
#define AJAY_GF2

#include <emmintrin.h>
#include <smmintrin.h>
#include <wmmintrin.h>

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/nt/GF2.hpp

__attribute__((target("pclmul"))) inline __m128i myclmul(
    const __m128i &a, const __m128i &b) {
  return _mm_clmulepi64_si128(a, b, 0);
}

// field with 2^n elements
template <int K>
struct GF2 {
  // https://oeis.org/A344141
  // irreducible poly x^K + ...
  static constexpr int POLY[65] = {0, 0, 3, 3, 3, 5, 3, 3, 27, 3, 9, 5, 9, 27,
      33, 3, 43, 9, 9, 39, 9, 5, 3, 33, 27, 9, 27, 39, 3, 5, 3, 9, 141, 75, 27,
      5, 53, 63, 99, 17, 57, 9, 39, 89, 33, 27, 3, 33, 45, 113, 29, 75, 9, 71,
      125, 71, 149, 17, 99, 123, 3, 39, 105, 3, 27};

  static constexpr uint64_t mask() { return uint64_t(-1) >> (64 - K); }

  __attribute__((target("sse4.2"))) static uint64_t mul(uint64_t a, uint64_t b) {
    static bool prepared = 0;
    static uint64_t MEMO[8][65536];
    if (!prepared) {
      prepared = 1;
      vector<uint64_t> tmp(128);
      tmp[0] = 1;
      For(i, 127) {
        tmp[i + 1] = tmp[i] << 1;
        if (tmp[i] >> (K - 1) & 1) {
          tmp[i + 1] ^= POLY[K];
          tmp[i + 1] &= mask();
        }
      }
      For(k, 8) {
        MEMO[k][0] = 0;
        For(i, 16) {
          For(s, 1 << i) { MEMO[k][s | 1 << i] = MEMO[k][s] ^ tmp[16 * k + i]; }
        }
      }
    }
    const __m128i a_ = _mm_set_epi64x(0, a);
    const __m128i b_ = _mm_set_epi64x(0, b);
    const __m128i c_ = myclmul(a_, b_);
    uint64_t lo = _mm_extract_epi64(c_, 0);
    uint64_t hi = _mm_extract_epi64(c_, 1);
    uint64_t x = 0;
    x ^= MEMO[0][lo & 65535];
    x ^= MEMO[1][(lo >> 16) & 65535];
    x ^= MEMO[2][(lo >> 32) & 65535];
    x ^= MEMO[3][(lo >> 48) & 65535];
    x ^= MEMO[4][hi & 65535];
    x ^= MEMO[5][(hi >> 16) & 65535];
    x ^= MEMO[6][(hi >> 32) & 65535];
    x ^= MEMO[7][(hi >> 48) & 65535];
    return x;
  }

  uint64_t val;
  constexpr GF2(const uint64_t val = 0) noexcept : val(val & mask()) {}
  bool operator<(const GF2 &other) const {
    return val < other.val;
  }  // To use std::map
  GF2 &operator+=(const GF2 &p) {
    val ^= p.val;
    return *this;
  }
  GF2 &operator-=(const GF2 &p) {
    val ^= p.val;
    return *this;
  }
  GF2 &operator*=(const GF2 &p) {
    val = mul(val, p.val);
    return *this;
  }

  GF2 &operator/=(const GF2 &p) {
    *this *= p.inverse();
    return *this;
  }
  GF2 operator-() const { return *this; }
  GF2 operator+(const GF2 &p) const { return GF2(*this) += p; }
  GF2 operator-(const GF2 &p) const { return GF2(*this) -= p; }
  GF2 operator*(const GF2 &p) const { return GF2(*this) *= p; }
  GF2 operator/(const GF2 &p) const { return GF2(*this) /= p; }
  bool operator==(const GF2 &p) const { return val == p.val; }
  bool operator!=(const GF2 &p) const { return val != p.val; }
  GF2 inverse() const { return pow((uint64_t(1) << K) - 2); }
  GF2 pow(uint64_t n) const {
    GF2 ret(1), mul(val);
    while (n > 0) {
      if (n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
};

#ifdef FASTIO
template <int K>
void rd(GF2<K> &x) {
  io::rd(x.val);
  x.val &= GF2<K>::mask();
}
template <int K>
void wt(GF2<K> x) {
  io::wt(x.val);
}
#endif
} // namespace mitsuha
#endif // AJAY_GF2
