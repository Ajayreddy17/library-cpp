#ifndef AJAY_POINT_ADD_RECT_SUM
#define AJAY_POINT_ADD_RECT_SUM

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

namespace mitsuha{
template <typename AbelGroup, typename XY, bool SMALL_X = false>
struct Point_Add_Rectangle_Sum {
    using G = typename AbelGroup::value_type;
    using Point = tuple<XY, XY, G>;
    vector<Point> point;
    vector<tuple<XY, XY, XY, XY>> rect;

    Point_Add_Rectangle_Sum() {}

    void add_query(XY x, XY y, G w) { point.emplace_back(x, y, w); }
    void sum_query(XY xl, XY xr, XY yl, XY yr) { rect.emplace_back(xl, xr, yl, yr); }

    vector<G> calc() {
        int N = point.size(), Q = rect.size();
        if (N == 0 || Q == 0) return vector<G>(Q, AbelGroup::unit());
        int NX = 0;
        if (!SMALL_X) {
            sort(point.begin(), point.end(), [&](auto &x, auto &y) -> bool { return get<0>(x) < get<0>(y); });
            vector<XY> keyX;
            keyX.reserve(N);
            for (auto &&[a, b, c]: point) {
                if (len(keyX) == 0 || keyX.back() != a) { keyX.emplace_back(a); }
                a = len(keyX) - 1;
            }
            for (auto &&[xl, xr, yl, yr]: rect) {
                xl = lower_bound(keyX.begin(), keyX.end(), xl) - keyX.begin();
                xr = lower_bound(keyX.begin(), keyX.end(), xr) - keyX.begin();
            }
            NX = len(keyX);
        }
        if (SMALL_X) {
            XY mx = numeric_limits<XY>::max() / 2;
            for (auto &&[x, y, g]: point) chmin(mx, x);
            for (auto &&[x, y, g]: point) x -= mx, chmax(NX, x + 1);
            for (auto &&[xl, xr, yl, yr]: rect) {
                xl -= mx, xr -= mx;
                xl = max(0, min<int>(xl, NX));
                xr = max(0, min<int>(xr, NX));
            }
        }

        vector<tuple<XY, int, int, int>> event(Q + Q);
        for(int q = 0; q < Q; q ++) {
            auto &[xl, xr, yl, yr] = rect[q];
            event[2 * q] = {yl, xl, xr, 2 * q};
            event[2 * q + 1] = {yr, xl, xr, 2 * q + 1};
        }
        sort(point.begin(), point.end(), [&](auto &x, auto &y) -> bool { return get<1>(x) < get<1>(y); });
        sort(event.begin(), event.end(), [&](auto &x, auto &y) -> bool { return get<0>(x) < get<0>(y); });
        FenwickTree<AbelGroup> bit(NX);
        vector<G> res(Q, AbelGroup::unit());
        int j = 0;
        for (auto &&[y, xl, xr, qq]: event) {
            while (j < N && get<1>(point[j]) < y) {
                bit.add(get<0>(point[j]), get<2>(point[j]));
                ++j;
            }
            G g = bit.sum(xl, xr);
            int q = qq / 2;
            if (qq % 2 == 0) g = AbelGroup::inverse(g);
            res[q] = AbelGroup::op(res[q], g);
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_POINT_ADD_RECT_SUM