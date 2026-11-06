#ifndef AJAY_MAX_OF_LINEAR_SEGMENTS
#define AJAY_MAX_OF_LINEAR_SEGMENTS

#include "library/mod/min_of_linear_segments.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/max_of_linear_segments.hpp
// same as min_of_linear_segments, for the x that update the prefix maximum of (ax + b) mod mod
pair<vector<int>, vector<int>> max_of_linear_segments(int a, int b, int mod) {
    a = (a == 0 ? 0 : mod - a);
    b = mod - 1 - b;
    return min_of_linear_segments(a, b, mod);
}
} // namespace mitsuha
#endif // AJAY_MAX_OF_LINEAR_SEGMENTS
