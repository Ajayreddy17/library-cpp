#ifndef AJAY_INCREMENTAL_RECT_UNION
#define AJAY_INCREMENTAL_RECT_UNION

#include "library/datastructure/fastset.hpp"

namespace mitsuha{
// [0, x] x [0, y] Add -> get union area
template <typename XY, typename AREA, bool SMALL_XY>
struct Incremental_Rectangle_Union {
    FastSet ss;
    vector<XY> ht;
    map<XY, XY> MP; // right end -> height
    AREA area;

    Incremental_Rectangle_Union() : area(AREA(0)) {
        static_assert(!SMALL_XY);
        MP[0] = numeric_limits<XY>::max() / 2, MP[numeric_limits<XY>::max() / 2] = 0;
    }

    Incremental_Rectangle_Union(int LIM): ss(LIM + 1), ht(LIM + 1), area(AREA(0)) {
        static_assert(SMALL_XY);
        ht[0] = numeric_limits<XY>::max() / 2, ht[LIM] = 0, ss.insert(0), ss.insert(LIM);
    }

    AREA add(XY x, XY y) {
        if constexpr (SMALL_XY) add_fast(x, y);
        if constexpr (!SMALL_XY) add_MP(x, y);
        return area;
    }

    void reset() {
        area = 0;
        if constexpr (SMALL_XY) {
            int LIM = len(ss) - 1;
            ss.enumerate(0, LIM + 1, [&](int i) -> void { ss.erase(i); });
            ht[0] = numeric_limits<XY>::max() / 2, ht[LIM] = 0, ss.insert(0), ss.insert(LIM);
        } else {
            MP.clear(), MP[0] = numeric_limits<XY>::max() / 2, MP[numeric_limits<XY>::max() / 2] = 0;
        }
    }

private:
    void add_MP(XY x, XY y) {
        if (x == 0 || y == 0) return;
        auto it = MP.lower_bound(x);
        auto [rx, ry] = *it;
        if (ry >= y) return;
        // split
        if (x < rx) MP[x] = ry;
        it = MP.find(x);
        while (1) {
            auto [x2, y2] = *it;
            it = prev(MP.erase(it));
            auto [x1, y1] = *it;
            // [x1,x2]: y2 -> 0
            area -= AREA(x2 - x1) * AREA(y2);
            if (y1 >= y) break;
        }
        auto [x1, y1] = *it;
        // [x1, x]: 0 -> y
        MP[x] = y, area += AREA(x - x1) * AREA(y);
        return;
    }

    void add_fast(XY x, XY y) {
        if (x == 0 || y == 0) return;
        int rx = ss.next(x);
        int ry = ht[rx];
        if (ry >= y) return;
        // split
        if (x < rx) ss.insert(x), ht[x] = ry;
        int x2 = x;
        while (1) {
            XY y2 = ht[x2];
            ss.erase(x2);
            int x1 = ss.prev(x2);
            XY y1 = ht[x1];
            // [x1,x2]: y2 -> 0
            area -= AREA(x2 - x1) * AREA(y2);
            x2 = x1;
            if (y1 >= y) break;
        }
        ss.insert(x), ht[x] = y, area += AREA(x - x2) * AREA(y);
        return;
    }
};
} // namespace mitsuha
#endif // AJAY_INCREMENTAL_RECT_UNION