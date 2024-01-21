#ifndef AJAY_CONVEXHULL
#define AJAY_CONVEXHULL

#include "library/geo/base.hpp"

namespace mitsuha{
template <typename T>
vector<int> ConvexHull(vector<pair<T, T>>& XY, string mode = "full", bool inclusive = false, bool sorted = false) {
    assert(mode == "full" || mode == "lower" || mode == "upper");
    long long N = XY.size();
    if (N == 1) return {0};
    if (N == 2) return {0, 1};
    vector<int> I((int)(XY.size()));
    iota(I.begin(), I.end(), 0);
    sort(I.begin(), I.end(), [&](int i, int j) { return (XY[i] == XY[j] ? i < j : XY[i] < XY[j]); });

    auto check = [&](long long i, long long j, long long k) -> bool {
        auto xi = XY[i].first, yi = XY[i].second;
        auto xj = XY[j].first, yj = XY[j].second;
        auto xk = XY[k].first, yk = XY[k].second;
        auto dx1 = xj - xi, dy1 = yj - yi;
        auto dx2 = xk - xj, dy2 = yk - yj;
        T det = dx1 * dy2 - dy1 * dx2;
        return (inclusive ? det >= 0 : det > 0);
    };

    auto calc = [&]() {
        vector<int> P;
        for (auto&& k: I) {
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
    if ((int)(P.size()) >= 2 && P[0] == P.back()) P.pop_back();
    return P;
}

template <typename T>
vector<int> ConvexHull(vector<Point<T>>& XY, string mode = "full", bool inclusive = false, bool sorted = false) {
    assert(mode == "full" || mode == "lower" || mode == "upper");
    long long N = XY.size();
    if (N == 1) return {0};
    if (N == 2) return {0, 1};
    vector<int> I((int) (XY.size()));
    iota(I.begin(), I.end(), 0);
    sort(I.begin(), I.end(), [&](int i, int j) { return (XY[i] == XY[j] ? i < j : XY[i] < XY[j]); });

    auto check = [&](long long i, long long j, long long k) -> bool {
        auto xi = XY[i].x, yi = XY[i].y;
        auto xj = XY[j].x, yj = XY[j].y;
        auto xk = XY[k].x, yk = XY[k].y;
        auto dx1 = xj - xi, dy1 = yj - yi;
        auto dx2 = xk - xj, dy2 = yk - yj;
        T det = dx1 * dy2 - dy1 * dx2;
        return (inclusive ? det >= 0 : det > 0);
    };

    auto calc = [&]() {
        vector<int> P;
        for (auto &&k: I) {
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
    if ((int) (P.size()) >= 2 && P[0] == P.back()) P.pop_back();
    return P;
}
} // namespace mitsuha
#endif // AJAY_CONVEXHULL