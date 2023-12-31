#ifndef AJAY_RECT_ADD_RECT_SUM
#define AJAY_RECT_ADD_RECT_SUM

#include "library/datastructure/offline_query/coefficient_query_2d.hpp"

namespace mitsuha{
template <typename T, typename XY>
struct Rectangle_Add_Rectangle_Sum {
    Coefficient_Query_2D<2, 2, T, XY> CQ;
    void add_query(XY x1, XY x2, XY y1, XY y2, T w) {
        CQ.add_query(x1, y1, w), CQ.add_query(x1, y2, -w);
        CQ.add_query(x2, y1, -w), CQ.add_query(x2, y2, w);
    }
    void sum_query(XY x1, XY x2, XY y1, XY y2) {
        --x1, --y1, --x2, --y2;
        CQ.sum_query(x1, y1), CQ.sum_query(x1, y2);
        CQ.sum_query(x2, y1), CQ.sum_query(x2, y2);
    }
    vector<T> calc() {
        vector<T> tmp = CQ.calc();
        int Q = len(tmp) / 4;
        vector<T> res(Q);
        for(int q = 0; q < Q; ++q) {
            res[q] = tmp[4 * q] - tmp[4 * q + 1] - tmp[4 * q + 2] + tmp[4 * q + 3];
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_RECT_ADD_RECT_SUM