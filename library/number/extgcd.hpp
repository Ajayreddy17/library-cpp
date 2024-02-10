#ifndef AJAY_EXTGCD
#define AJAY_EXTGCD

namespace mitsuha{
/*
Farey Find the left and right sides of the first occurrence of a/b in a number sequence.
a/b = 19/12 → (x1/y1, x2/y2) = (11/7, 8/5) → (11,7,8,5)。
*/
tuple<long long, long long, long long, long long> Farey_lr(long long a, long long b) {
    assert(a > 0 && b > 0);
    if (a == b) return {0, 1, 1, 0};
    long long q = (a - 1) / b;
    auto [x1, y1, x2, y2] = Farey_lr(b, a - q * b);
    return {q * x2 + y2, x2, q * x1 + y1, x1};
}

// ax + by = d returns the minimum solution (x, y, d).
// (|x|+|y|, x) Let be the lexicographical minimum for.
tuple<long long, long long, long long> extgcd(long long a, long long b) {
    auto [x1, y1, x2, y2] = Farey_lr(a, b);
    tie(x1, y1) = make_pair(y1, -x1);
    tie(x2, y2) = make_pair(-y2, x2);
    long long g = a * x1 + b * y1;
    pair<long long, long long> key1 = make_pair(abs(x1) + abs(y1), x1);
    pair<long long, long long> key2 = make_pair(abs(x2) + abs(y2), x2);
    return (key1 < key2 ? make_tuple(x1, y1, g) : make_tuple(x2, y2, g));
}
} // namespace mitsuha
#endif // AJAY_EXTGCD