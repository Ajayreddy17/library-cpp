#ifndef AJAY_PICK_FORMULA
#define AJAY_PICK_FORMULA


// input: simple polygon
// return: area2, inner, boundary
namespace mitsuha{
template <typename T>
tuple<int128, int128, int128> pick_formula(vector<Point<T>>& A) {
  using P = Point<T>;
  int N = len(A);
  int128 area2 = 0;
  int128 bd = 0;
  For(i, N) {
    P a = A[i], b = A[(i + 1) % N];
    area2 += int128(a.x) * b.y - int128(a.y) * b.x;
    P d = b - a;
    bd += gcd(d.x, d.y);
  }
  int128 in = (area2 + 2 - bd) / 2;
  return {area2, in, bd};
}

} // namespace mitsuha
#endif // AJAY_PICK_FORMULA
