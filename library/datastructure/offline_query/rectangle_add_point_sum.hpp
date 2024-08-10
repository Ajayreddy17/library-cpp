#ifndef RECT_ADD_POINT_SUM
#define RECT_ADD_POINT_SUM

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

namespace mitsuha{
template<typename AbelGroup, typename XY, bool SMALL_X = false>
struct Rectangle_Add_Point_Sum {
    using G = typename AbelGroup::value_type;
    vector<tuple<XY, XY, XY, G>> rect;
    vector<tuple<int, XY, XY>> point;

    Rectangle_Add_Point_Sum() {}

    void add_query(XY x1, XY x2, XY y1, XY y2, G g) {
        rect.emplace_back(y1, x1, x2, g), rect.emplace_back(y2, x2, x1, g);
    }
    void sum_query(XY x, XY y) { point.emplace_back(point.size(), x, y); }
    
    vector<G> calc() {
        int N = rect.size(), Q = point.size();
        if (N == 0 || Q == 0) return vector<G>(Q, AbelGroup::unit());
        int NX = 0;
        if (!SMALL_X) {
            sort(point.begin(),point.end(),
                 [&](auto &x, auto &y) -> bool { return get<1>(x) < get<1>(y); });
            vector<XY> keyX;
            keyX.reserve(Q);
            for (auto &&[i, a, b]: point) {
                if (keyX.size() == 0 || keyX.back() != a) { keyX.emplace_back(a); }
                a = keyX.size() - 1;
            }
            for (auto &&[y, x1, x2, g]: rect) {
                x1 = distance(keyX.begin(), lower_bound(keyX.begin(), keyX.end(), x1));
                x2 = distance(keyX.begin(), lower_bound(keyX.begin(), keyX.end(), x2));
            }
            NX = keyX.size();
        }
        if (SMALL_X) {
            XY mx = numeric_limits<XY>::max();
            for (auto &&[i, x, y]: point) mx = min(mx, x);
            for (auto &&[i, x, y]: point) x -= mx, NX = max((long long) NX, x + 1);
            for (auto &&[y, x1, x2, g]: rect) {
                x1 -= mx, x2 -= mx;
                x1 = max(0, min<int>(x1, NX)), x2 = max(0, min<int>(x2, NX));
            }
        }

        sort(point.begin(), point.end(), [&](auto &x, auto &y) -> bool { return get<2>(x) < get<2>(y); });
        sort(rect.begin(), rect.end(), [&](auto &x, auto &y) -> bool { return get<0>(x) < get<0>(y); });
        FenwickTree<AbelGroup> bit(NX);
        vector<G> res(Q, AbelGroup::unit());
        int j = 0;
        for (long long i = 0; i < Q; i++) {
            auto [q, x, y] = point[i];
            while (j < N && get<0>(rect[j]) <= y) {
                auto [yy, x1, x2, g] = rect[j++];
                bit.add(x1, g), bit.add(x2, AbelGroup::inverse(g));
            }
            res[q] = bit.sum(x + 1);
        }
        return res;
    }
};
} // namespace mitsuha
#endif // RECT_ADD_POINT_SUM