#ifndef AJAY_OFFSET_VECTOR
#define AJAY_OFFSET_VECTOR


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/ds/offset_vector.hpp
// stores V[L],...,V[R-1] in a vector of size R-L
template <typename T>
struct OffsetVector {
  int off;
  vector<T> dat;
  OffsetVector(int L = 0, int R = 0, T t = {}) : off(L), dat(R - L, t) {}
  T& operator[](int i) { return dat[i - off]; }
  const T& operator[](int i) const { return dat[i - off]; }
  int size() const { return dat.size(); }
  int L() const { return off; }
  int R() const { return off + (dat.size()); }
};
} // namespace mitsuha
#endif // AJAY_OFFSET_VECTOR
