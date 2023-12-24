#ifndef AJAY_MERGE_VECTOR_SPACE
#define AJAY_MERGE_VECTOR_SPACE

#include "library/linear_algebra/xor/vector_space.hpp"

namespace mitsuha{
template <typename UINT>
struct Merge_Vector_Space {
  using value_type = Vector_Space<UINT>;
  using X = value_type;
  static X op(X x, X y) { return Vector_Space<UINT>::merge(x, y); }
  static constexpr X unit() { return X{}; }
  static constexpr bool commute = 1;
};
} // namespace mitsuha
#endif // AJAY_MERGE_VECTOR_SPACE