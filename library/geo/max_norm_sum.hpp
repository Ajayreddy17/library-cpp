#ifndef AJAY_MAX_NORM_SUM
#define AJAY_MAX_NORM_SUM

#include "library/geo/base.hpp"
#include "library/geo/angle_sort.hpp"

namespace mitsuha{
// A sequence of vectors is given. Select a subsequence to minimize the norm of the sum.
// Be careful to ensure that the squared sum of the coordinates, SM, does not overflow
// https://atcoder.jp/contests/abc139/tasks/abc139_f
// https://codeforces.com/contest/1841/problem/F
template <typename SM, typename T>
pair<SM, vector<int>> max_norm_sum(vector<Point<T>> dat) {
    auto I = angle_sort(dat);
    {
        vector<int> J;
        for (auto&& i : I) {
            if (dat[i].x != 0 || dat[i].y != 0) J.emplace_back(i);
        }
        swap(I, J);
    }
    dat = rearrange(dat, I);
    const int N = len(dat);

    if (N == 0) { return {0, {}}; }
    SM ANS = 0;
    pair<int, int> LR = {0, 0};

    int L = 0, R = 1;
    Point<T> c = dat[0];
    auto eval = [&]() -> SM { return SM(c.x) * c.x + SM(c.y) * c.y; };
    if (chmax(ANS, eval())) LR = {L, R};

    while (L < N) {
        Point<T>&A = dat[L], &B = dat[R % N];
        if (R - L < N && (A.det(B) > 0 || (A.det(B) == 0 && A.dot(B) > 0))) {
            c = c + B;
            R++;
            if (chmax(ANS, eval())) LR = {L, R};
        } else {
            c = c - A;
            L++;
            if (chmax(ANS, eval())) LR = {L, R};
        }
    }
    vector<int> ids;
    For(i, LR.first, LR.second) { ids.emplace_back(I[i % N]); }
    return {ANS, ids};
}
} // namespace mitsuha
#endif // AJAY_MAX_NORM_SUM