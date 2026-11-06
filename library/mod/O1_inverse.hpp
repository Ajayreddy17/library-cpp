#ifndef AJAY_O1_INVERSE
#define AJAY_O1_INVERSE

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/O1_inverse.hpp
// https://qoj.ac/problem/5
// inverse mod a prime p <= 2^30: O(p^{2/3}) precomputation, O(1) per query
// 10^8 queries: 3 sec
template <typename mint>
mint O1_inverse(int a) {
    /*
    take n with n^3 >= p and precompute inverses up to n^2.
    approximate a/p by x/y with |a/p - x/y| <= 1/(ny); then
    |ay - px| <= p/n <= n^2, so (ay - px)^{-1} = (ay)^{-1} is in the table.
    x/y is one of the two neighbours of a/p in the Farey sequence of order n.
    with n^2 buckets the fractions are distinct per bucket; keep the better neighbour.
    */
    static int p = 0;
    static double cp = 0.0;
    static vector<int> FRAC;
    static vector<mint> invs;
    if (p != mint::get_mod()) {
        p = mint::get_mod();
        int k = min(2 << 20, p);
        invs.resize(k);
        invs[1] = 1;
        For(i, 2, k) {
            int q = (p + i - 1) / i;
            invs[i] = invs[i * q - p] * mint::raw(q);
        }
        assert(p <= (1 << 30));
        FRAC.assign(1 << 20, -1);
        cp = 1.0 * (1 << 20) / p;
        for (int y = 1023; y >= 1; --y) {
            for (int x = 0; x < y; ++x) { FRAC[(x << 20) / y] = {x << 10 | y}; }
        }
        For(i, 1, len(FRAC)) if (FRAC[i] == -1) FRAC[i] = FRAC[i - 1];
        int nxt = 1025;
        Frr(i, 1, len(FRAC)) {
            if (FRAC[i] != FRAC[i - 1]) {
                nxt = FRAC[i];
                continue;
            }
            int x1 = FRAC[i] >> 10, y1 = FRAC[i] & 1023;
            int x2 = nxt >> 10, y2 = nxt & 1023;
            int c1 = i * y1 - (x1 << 20);
            int c2 = i * y2 - (x2 << 20);
            if (abs(c2) < abs(c1)) FRAC[i] = nxt;
        }
    }
    assert(0 <= a && a < p);
    if (a < len(invs)) return invs[a];

    int k = a * cp;
    long long x = FRAC[k] >> 10, y = FRAC[k] & 1023;
    int c = (long long)(a) * y - (long long)(p) * x;
    if (c < 0) c = -c, y = -y;
    return invs[c] * mint(y);
}
} // namespace mitsuha
#endif // AJAY_O1_INVERSE
