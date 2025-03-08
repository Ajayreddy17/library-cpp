#ifndef AJAY_FENWICKTREE_2D
#define AJAY_FENWICKTREE_2D

#include "library/monoid/monoid_add.hpp"

namespace mitsuha {
template <typename Monoid, typename XY, bool SMALL_X = false>
struct FenwickTree_2D {
    using G = Monoid;
    using E = typename G::value_type;
    static_assert(G::commute);
    int N;
    vector<XY> keyX;
    XY min_X;
    vector<int> indptr;
    vector<XY> keyY;
    vector<E> dat;

    FenwickTree_2D() {}
    FenwickTree_2D(vector<XY>& X, vector<XY>& Y) { build(X, Y); }
    FenwickTree_2D(vector<XY>& X, vector<XY>& Y, vector<E> wt) { build(X, Y, wt); }

    void build(vector<XY> X, vector<XY> Y) {
        assert(len(X) == len(Y));
        if constexpr (!SMALL_X) {
            keyX = X;
            sort(keyX.begin(), keyX.end());
            keyX.erase(unique(keyX.begin(), keyX.end()), keyX.end());
            keyX.shrink_to_fit();
            N = len(keyX);
        } else {
            min_X = (len(X) == 0 ? 0 : *min_element(X.begin(), X.end()));
            N = (len(X) == 0 ? 0 : *max_element(X.begin(), X.end())) - min_X + 1;
            keyX.resize(N);
            For(i, N) keyX[i] = min_X + i;
        }

        auto I = argsort(Y);
        X = rearrange(X, I), Y = rearrange(Y, I);

        For(i, len(X)) X[i] = xtoi(X[i]);

        vector<XY> last_y(N, (numeric_limits<XY>::min() / 2) - 1);
        indptr.assign(N + 1, 0);
        For(i, len(X)) {
            int ix = X[i];
            XY y = Y[i];
            while (ix < N) {
                if (last_y[ix] == y) break;
                last_y[ix] = y, indptr[ix + 1]++, ix = nxt(ix);
            }
        }
        For(i, N) indptr[i + 1] += indptr[i];
        keyY.resize(indptr.back());
        dat.assign(indptr.back(), G::unit());
        fill(last_y.begin(), last_y.end(), (numeric_limits<XY>::min() / 2) - 1);
        vector<int> prog = indptr;
        For(i, len(X)) {
            int ix = X[i];
            XY y = Y[i];
            while (ix < N) {
                if (last_y[ix] == y) break;
                last_y[ix] = y, keyY[prog[ix]++] = y, ix = nxt(ix);
            }
        }
    }

    void build(vector<XY> X, vector<XY> Y, vector<E> wt) {
        assert(len(X) == len(Y));
        if constexpr (!SMALL_X) {
            keyX = X;
            sort(keyX.begin(), keyX.end());
            keyX.erase(unique(keyX.begin(), keyX.end()), keyX.end());
            keyX.shrink_to_fit();
            N = len(keyX);
        } else {
            min_X = (len(X) == 0 ? 0 : *min_element(X.begin(), X.end()));
            N = (len(X) == 0 ? 0 : *max_element(X.begin(), X.end())) - min_X + 1;
            keyX.resize(N);
            For(i, N) keyX[i] = min_X + i;
        }

        auto I = argsort(Y);
        X = rearrange(X, I), Y = rearrange(Y, I), wt = rearrange(wt, I);

        For(i, len(X)) X[i] = xtoi(X[i]);

        vector<XY> last_y(N, (numeric_limits<XY>::min() / 2) - 1);
        indptr.assign(N + 1, 0);
        For(i, len(X)) {
            int ix = X[i];
            XY y = Y[i];
            while (ix < N) {
                if (last_y[ix] == y) break;
                last_y[ix] = y, indptr[ix + 1]++, ix = nxt(ix);
            }
        }
        For(i, N) indptr[i + 1] += indptr[i];
        keyY.resize(indptr.back());
        dat.assign(indptr.back(), G::unit());
        fill(last_y.begin(), last_y.end(), (numeric_limits<XY>::min() / 2) - 1);
        vector<int> prog = indptr;
        For(i, len(X)) {
            int ix = X[i];
            XY y = Y[i];
            E w = wt[i];
            while (ix < N) {
                if (last_y[ix] != y) {
                    last_y[ix] = y, keyY[prog[ix]] = y, dat[prog[ix]] = w;
                    prog[ix]++;
                } else {
                    dat[prog[ix] - 1] = G::op(dat[prog[ix] - 1], w);
                }
                ix = nxt(ix);
            }
        }
        For(i, N) {
            int n = indptr[i + 1] - indptr[i];
            For(j, n - 1) {
                int k = nxt(j);
                if (k < n)
                    dat[indptr[i] + k] = G::op(dat[indptr[i] + k], dat[indptr[i] + j]);
            }
        }
    }

    void add(XY x, XY y, E val) { multiply(x, y, val); }
    void multiply(XY x, XY y, E val) {
        int i = xtoi(x);
        assert(keyX[i] == x);
        while (i < N) { multiply_i(i, y, val), i = nxt(i); }
    }

    E sum(XY lx, XY rx, XY ly, XY ry) { return prod(lx, rx, ly, ry); }
    E prod(XY lx, XY rx, XY ly, XY ry) {
        E pos = G::unit(), neg = G::unit();
        int L = xtoi(lx) - 1, R = xtoi(rx) - 1;
        while (L < R) { pos = G::op(pos, prod_i(R, ly, ry)), R = prv(R); }
        while (R < L) { neg = G::op(neg, prod_i(L, ly, ry)), L = prv(L); }
        return G::op(pos, G::inverse(neg));
    }

    E prefix_sum(XY rx, XY ry) { return prefix_prod(rx, ry); }
    E prefix_prod(XY rx, XY ry) {
        E pos = G::unit();
        int R = xtoi(rx) - 1;
        while (R >= 0) { pos = G::op(pos, prefix_prod_i(R, ry)), R = prv(R); }
        return pos;
    }

private:

    inline int xtoi(XY x) {
        if constexpr (SMALL_X) {
            return clamp<int>(x - min_X, 0, N);
        } else {
            return lower_bound(keyX.begin(), keyX.end(), x) - keyX.begin();
        }
    }
    inline int nxt(int i) { return i + ((i + 1) & -(i + 1)); }
    inline int prv(int i) { return i - ((i + 1) & -(i + 1)); }

    void multiply_i(int i, XY y, E val) {
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        auto it = keyY.begin() + LID;
        int j = lower_bound(it, it + n, y) - it;
        while (j < n) { dat[LID + j] = G::op(dat[LID + j], val), j = nxt(j); }
    }

    E prod_i(int i, XY ly, XY ry) {
        E pos = G::unit(), neg = G::unit();
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        auto it = keyY.begin() + LID;
        int L = lower_bound(it, it + n, ly) - it - 1;
        int R = lower_bound(it, it + n, ry) - it - 1;
        while (L < R) { pos = G::op(pos, dat[LID + R]), R = prv(R); }
        while (R < L) { neg = G::op(neg, dat[LID + L]), L = prv(L); }
        return G::op(pos, G::inverse(neg));
    }

    E prefix_prod_i(int i, XY ry) {
        E pos = G::unit();
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        auto it = keyY.begin() + LID;
        int R = lower_bound(it, it + n, ry) - it - 1;
        while (R >= 0) { pos = G::op(pos, dat[LID + R]), R = prv(R); }
        return pos;
    }
};
} // namespace mitsuha
#endif // AJAY_FENWICKTREE_2D