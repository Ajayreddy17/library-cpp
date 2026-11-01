#ifndef AJAY_POWER_QUERY
#define AJAY_POWER_QUERY

#include "library/monoid/monoid_mul.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/ds/power_query.hpp

// powers of a fixed element; precomputes blocks of B powers
template <typename Mono, int B = 1024>
struct Power_Query {
  static_assert(2 <= B);
  using X = typename Mono::value_type;
  vector<vector<X>> dat;

  Power_Query(X a) { dat.emplace_back(make_pow(a)); }

  X operator()(long long n) {
    X res = Mono::unit();
    int k = 0;
    while (n) {
      int r = n % B;
      n /= B;
      if (len(dat) == k) {
        dat.emplace_back(make_pow(dat[k - 1].back()));
      }
      res = Mono::op(res, dat[k][r]);
      ++k;
    }
    return res;
  }

  // list the factors multiplied for the n-th power; usable e.g. for matrix-vector products
  vector<X> get_list(long long n) {
    vector<X> lst;
    int k = 0;
    while (n) {
      int r = n % B;
      n /= B;
      if (len(dat) == k) {
        dat.emplace_back(make_pow(dat[k - 1].back()));
      }
      lst.emplace_back(dat[k][r]);
      ++k;
    }
    return lst;
  }

  X operator[](long long n) { return (*this)(n); }

 private:
  vector<X> make_pow(X a) {
    vector<X> res = {Mono::unit()};
    For(B) { res.emplace_back(Mono::op(res.back(), a)); }
    return res;
  }
};
} // namespace mitsuha
#endif // AJAY_POWER_QUERY
