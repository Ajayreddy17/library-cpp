#ifndef AJAY_POLYGON
#define AJAY_POLYGON

#include "library/geo/base.hpp"
#include "library/geo/cross_point.hpp"

namespace mitsuha{
template <typename T>
struct Polygon {
    vector<Point<T>> point;
    T a;

    Polygon(vector<Point<T>> point) : point(point) { build(); }

    int size() { return len(point); }

    template <typename REAL>
    REAL area() {
        return a * 0.5;
    }

    T area_2() { return a; }

    bool is_convex() {
        For(j, len(point)) {
            int i = (j == 0 ? len(point) - 1 : j - 1);
            int k = (j == len(point) - 1 ? 0 : j + 1);
            if ((point[j] - point[i]).det(point[k] - point[j]) < 0) return false;
        }
        return true;
    }

    // Inner: 1, On: 0, Outside: -1.
    int side(Point<T> p) {
        int n = len(point);
        For(i, n) if (point[i] == p) return 0;
        For(i, n) {
            Point<T> A = point[i], B = point[(i + 1) % n];
            if ((p - A).det(B - A) != 0) continue;
            if ((p - A).dot(B - A) >= 0 && (p - B).dot(A - B) >= 0) return 0;
        }
        // Consider the intersection of p with a half-line extending in the x direction (+1, +eps)
        int cnt = 0;
        For(i, n) {
            Point<T> A = point[i], B = point[(i + 1) % n];
            For(2) {
                swap(A, B);
                if (A.y > p.y) continue;
                if (B.y <= p.y) continue;
                if ((A - p).det(B - p) > 0) ++cnt;
            }
        }
        return (cnt % 2 == 0 ? -1 : 1);
    }

    // point[i] side seen only in the vicinity of
    // https://github.com/maspypy/library/blob/main/geo/polygon_side.png
    int side_at(int i, Point<T> p) {
        int n = len(point);
        p -= point[i];
        if (p.x == T(0) && p.y == T(0)) return 0;
        Point<T> L = point[(i + 1) % n] - point[i];
        Point<T> R = point[(i + n - 1) % n] - point[i];
        auto sgn = [&](T x) -> int {
            if (x == T(0)) return 0;
            return (x > T(0) ? 1 : -1);
        };
        int x = sgn(L.det(p)) + sgn(p.det(R)) + sgn(R.det(L));
        if (x == 0) return x;
        return (x > 0 ? 1 : -1);
    }

    // Does the line segment pass through the interior and exterior?
    // https://atcoder.jp/contests/jag2016-domestic/tasks/jag2016secretspring_e
    // https://atcoder.jp/contests/JAG2014Spring/tasks/icpc2014spring_f
    pair<bool, bool> side_segment(Point<T> L, Point<T> R) {
        Segment<T> S(L, R);
        // First, a pattern that non-trivially intersects with a line segment
        int n = len(point);
        For(i, n) {
            Segment<T> S2(point[i], point[(i + 1) % n]);
            if (count_cross(S, S2, false) == 1) { return {1, 1}; }
        }
        bool in = 0, out = 0;
        if (side(L) == 1 || side(R) == 1) in = 1;
        if (side(L) == -1 || side(R) == -1) out = 1;
        For(i, n) {
            if (!S.contain(point[i])) continue;
            for (auto& p: {L, R}) {
                int k = side_at(i, p);
                if (k == 1) in = 1;
                if (k == -1) out = 1;
            }
        }
        return {in, out};
    }

    template <typename REAL>
    vector<Segment<REAL>> cross_line(Line<T> L, bool allow_perimeter) {
        static_assert(is_integral_v<T>);
        using RP = Point<REAL>;
        int N = len(point);
        vector<int> sd(N);
        auto sgn = [&](T x) -> int { return (x > 0 ? 1 : x < 0 ? -1 : 0); };

        For(i, N) sd[i] = sgn(L.eval(point[i]));
        vector<pair<RP, int>> event;

        For(i, N) {
            int j = (i + 1) % N;

            if (sd[i] == 0) {
                int k = (i + N - 1) % N;
                int c = sd[k] - sd[j];
                event.emplace_back(RP(point[i]), c);
            }

            if (sd[i] * sd[j] < 0) {
                RP p = cross_point<REAL>(L, Line<T>(point[i], point[j]));
                int c = sd[i] - sd[j];
                event.emplace_back(p, c);
            }
        }

        if (L.b != 0) {
            sort(event.begin(), event.end(), [&](auto& x, auto& y) {
                if (x.first.x != y.first.x) return x.first.x < y.first.x;
                return x.first.y < y.first.y;
            });
        } else {
            sort(event.begin(), event.end(), [&](auto& x, auto& y) {
                if (x.first.y != y.first.y) return x.first.y < y.first.y;
                return x.first.x < y.first.x;
            });
        }

        vector<Segment<REAL>> ANS;
        int in = 0;

        if (allow_perimeter) {
            int l = 0;
            For(i, len(event)) {
                in += event[i].second;
                if (i + 1 == len(event) || in == 0) {
                    ANS.emplace_back(event[l].first, event[i].first);
                    l = i + 1;
                }
            }
        } else {
            For(i, len(event)) {
                in += event[i].second;
                if (i + 1 < len(event) && abs(in) == 2) {
                    ANS.emplace_back(event[i].first, event[i + 1].first);
                }
            }
        }

        assert(in == 0);
        return ANS;
    }

private:
    void build() {
        a = 0;
        For(i, len(point)) {
            int j = (i + 1 == len(point) ? 0 : i + 1);
            a += point[i].det(point[j]);
        }
        assert(a > 0);
    }
};
} // namespace mitsuha
#endif // AJAY_POLYGON