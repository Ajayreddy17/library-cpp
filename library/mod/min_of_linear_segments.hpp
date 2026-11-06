#ifndef AJAY_MIN_OF_LINEAR_SEGMENTS
#define AJAY_MIN_OF_LINEAR_SEGMENTS

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/min_of_linear_segments.hpp
/*
f(x) = (ax + b) mod mod, x >= 0.
The x where f updates its prefix minimum form a union of arithmetic progressions.
Returns
- the boundaries x_0 = 0 < x_1 < ... < x_n of the progressions
- the common difference dx_i used between x_i and x_{i+1}
The last boundary x_n is the first x with f(x) = 0 (scaled by gcd).
*/
pair<vector<int>, vector<int>> min_of_linear_segments(int a, int b, int mod) {
    assert(0 <= a && a < mod);
    assert(0 <= b && b < mod);
    vector<int> X = {0};
    vector<int> DX;
    int g = gcd(a, mod);
    a /= g, b /= g, mod /= g;
    // p/q <= (mod-a)/mod <= r/s
    int p = 0, q = 1, r = 1, s = 1;
    int det_l = mod - a, det_r = a;
    int x = 0, y = b;

    while (y) {
        // update r/s
        int k = det_r / det_l;
        det_r %= det_l;
        if (det_r == 0) {
            --k;
            det_r = det_l;
        }
        r += k * p;
        s += k * q;
        while (1) {
            int k = max(0, cld(det_l - y, det_r));
            if (det_l - k * det_r <= 0) break;
            det_l -= k * det_r;
            p += k * r;
            q += k * s;
            // p/q <= a/mod: each step of q lowers y by (aq - p mod) = det_l
            k = y / det_l;
            y -= k * det_l;
            x += q * k;
            X.emplace_back(x);
            DX.emplace_back(q);
        }
        k = det_l / det_r;
        det_l -= k * det_r;
        p += k * r;
        q += k * s;
        assert(min(p, q, r, s) >= 0);
    }
    return {X, DX};
}
} // namespace mitsuha
#endif // AJAY_MIN_OF_LINEAR_SEGMENTS
