#ifndef AJAY_CONVEXHULL
#define AJAY_CONVEXHULL

#include "library/geo/base.hpp"

namespace mitsuha {
// allow_180 = true It breaks if there are identical coordinate points
// If full, I[0] becomes the minimum in the sorted order
template <typename T, bool allow_180 = false>
vector<int> ConvexHull(vector<Point<T>>& XY, string mode = "full", bool sorted = false) {
    assert(mode == "full" || mode == "lower" || mode == "upper");
    long long N = XY.size();
    if (N == 1) return {0};
    if (N == 2) {
        if (XY[0] < XY[1]) return {0, 1};
        if (XY[1] < XY[0]) return {1, 0};
        return {0};
    }
    vector<int> I(N);
    if (sorted) {
        For(i, N) I[i] = i;
    } else {
        I = argsort(XY);
    }
    if constexpr (allow_180) {
        For(i, N - 1) assert(XY[i] != XY[i + 1]);
    }

    auto check = [&](long long i, long long j, long long k) -> bool {
        T det = (XY[j] - XY[i]).det(XY[k] - XY[i]);
        if constexpr (allow_180) return det >= 0;
        return det > T(0);
    };

    auto calc = [&]() {
        vector<int> P;
        for (auto&& k : I) {
            while (P.size() > 1) {
                auto i = P[P.size() - 2];
                auto j = P[P.size() - 1];
                if (check(i, j, k)) break;
                P.pop_back();
            }
            P.emplace_back(k);
        }
        return P;
    };

    vector<int> P;
    if (mode == "full" || mode == "lower") {
        vector<int> Q = calc();
        P.insert(P.end(), Q.begin(), Q.end());
    }
    if (mode == "full" || mode == "upper") {
        if (!P.empty()) P.pop_back();
        reverse(I.begin(), I.end());
        vector<int> Q = calc();
        P.insert(P.end(), Q.begin(), Q.end());
    }
    if (mode == "upper") reverse(P.begin(), P.end());
    while (len(P) >= 2 && XY[P[0]] == XY[P.back()]) P.pop_back();
    return P;
}
} // namespace mitsuha
#endif // AJAY_CONVEXHULL