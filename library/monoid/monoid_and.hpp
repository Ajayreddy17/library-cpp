#ifndef AJAY_MONOID_AND
#define AJAY_MONOID_AND

namespace mitsuha{
template <typename X>
struct Monoid_And {
  using value_type = X;
  static X op(X x, X y) { return x & y; }
  static constexpr X unit() { return X(-1); };
  static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_AND