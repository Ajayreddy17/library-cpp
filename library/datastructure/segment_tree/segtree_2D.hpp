#ifndef AJAY_SEGTREE_2D
#define AJAY_SEGTREE_2D

namespace mitsuha{
// Even if there are duplicate points, they are set as separate points, etc.
template <typename Monoid, typename XY, bool SMALL_X = false>
struct SegTree_2D {
    using MX = Monoid;
    using S = typename MX::value_type;
    static_assert(MX::commute);
    int N;
    vector<XY> keyX; // X to idx
    int minX;
    vector<XY> all_Y; // top node point sequence of
    vector<int> pos;
    int NX, log, size; // segtree data
    vector<int> indptr;
    vector<S> dat;
    vector<int> to_left; // fractional cascading

    SegTree_2D(vector<XY>& X, vector<XY>& Y) : SegTree_2D(len(X), [&](int i) -> tuple<XY, XY, S> { return {X[i], Y[i], MX::unit()}; }) {}

    SegTree_2D(vector<XY>& X, vector<XY>& Y, vector<S>& vals) : SegTree_2D(len(X), [&](int i) -> tuple<XY, XY, S> { return {X[i], Y[i], vals[i]}; }) {}

    // f(i) = (x,y,val)
    template <typename F>
    SegTree_2D(int N, F f) {
        vector<XY> X(N), Y(N);
        vector<S> wt(N);
        for(int i = 0; i < N; i++) {
            auto [a, b, c] = f(i);
            X[i] = a, Y[i] = b, wt[i] = c;
        }
        if (!SMALL_X) {
            keyX = X;
            settify(keyX);
            NX = len(keyX);
        } else {
            minX = (X.empty() ? 0 : *min_element(X.begin(), X.end()));
            NX = (X.empty() ? 1 : *max_element(X.begin(), X.end()) - minX + 1);
        }

        log = 0;
        while ((1 << log) < NX) ++log;
        size = (1 << log);

        vector<int> IX(N);
        for(long long i = 0; i < N; i++) IX[i] = xtoi(X[i]);
        indptr.assign(2 * size, 0);
        for (auto i: IX) {
            i += size;
            while (i) indptr[i]++, i /= 2;
        }
        indptr = cumsum<int>(indptr);
        dat.assign(2 * indptr.back(), MX::unit());
        to_left.assign(indptr[size], 0);

        vector<int> ptr = indptr;
        vector<int> I = sorted_indices(len(Y), [&](int x){ return Y[x]; });
        pos.resize(N);
        for(long long i = 0; i < N; i++) pos[I[i]] = i;
        for (auto raw_idx: I) {
            int i = IX[raw_idx] + size;
            int j = -1;
            while (i) {
                int p = ptr[i];
                ptr[i]++;
                dat[indptr[i + 1] + p] = wt[raw_idx];
                if (j != -1) { to_left[p] = (j % 2 == 0); }
                j = i, i /= 2;
            }
        }
        to_left = cumsum<int>(to_left);

        for(long long i = 0; i < 2 * size; i++) {
            int off = 2 * indptr[i], n = indptr[i + 1] - indptr[i];
            for(int j = n - 1; j >= 1;  j--) {
                dat[off + j] = MX::op(dat[off + 2 * j + 0], dat[off + 2 * j + 1]);
            }
        }
        all_Y = Y;
        sort(all_Y.begin(), all_Y.end());
    }
    //of the initially given point pool index
    void multiply(int raw_idx, S val) {
        int i = 1, p = pos[raw_idx];
        while (1) {
            multiply_i(i, p - indptr[i], val);
            if (i >= size) break;
            int lc = to_left[p] - to_left[indptr[i]];
            int rc = (p - indptr[i]) - lc;
            if (to_left[p + 1] - to_left[p]) {
                p = indptr[2 * i + 0] + lc;
                i = 2 * i + 0;
            } else {
                p = indptr[2 * i + 1] + rc;
                i = 2 * i + 1;
            }
        }
    }

    //of the initially given point pool index
    void set(int raw_idx, S val) {
        int i = 1, p = pos[raw_idx];
        while (1) {
            set_i(i, p - indptr[i], val);
            if (i >= size) break;
            int lc = to_left[p] - to_left[indptr[i]];
            int rc = (p - indptr[i]) - lc;
            if (to_left[p + 1] - to_left[p]) {
                p = indptr[2 * i + 0] + lc;
                i = 2 * i + 0;
            } else {
                p = indptr[2 * i + 1] + rc;
                i = 2 * i + 1;
            }
        }
    }

    S prod(XY lx, XY rx, XY ly, XY ry) {
        assert(lx <= rx && ly <= ry);
        int L = xtoi(lx), R = xtoi(rx);
        S res = MX::unit();
        auto dfs = [&](auto& dfs, int i, int l, int r, int a, int b) -> void {
            if (a == b || R <= l || r <= L) return;
            if (L <= l && r <= R) {
                res = MX::op(res, prod_i(i, a, b));
                return;
            }
            int la = to_left[indptr[i] + a] - to_left[indptr[i]];
            int ra = a - la;
            int lb = to_left[indptr[i] + b] - to_left[indptr[i]];
            int rb = b - lb;
            int m = (l + r) / 2;
            dfs(dfs, 2 * i + 0, l, m, la, lb);
            dfs(dfs, 2 * i + 1, m, r, ra, rb);
        };
        dfs(dfs, 1, 0, size, lower_bound(all_Y.begin(), all_Y.end(), ly) - all_Y.begin(), lower_bound(all_Y.begin(), all_Y.end(), ry) - all_Y.begin());
        return res;
    }

    // Count all points within a rectangle, logN
    int count(XY lx, XY rx, XY ly, XY ry) {
        assert(lx <= rx && ly <= ry);
        int L = xtoi(lx), R = xtoi(rx);
        int res = 0;
        auto dfs = [&](auto& dfs, int i, int l, int r, int a, int b) -> void {
            if (a == b || R <= l || r <= L) return;
            if (L <= l && r <= R) {
                res += b - a;
                return;
            }
            int la = to_left[indptr[i] + a] - to_left[indptr[i]];
            int ra = a - la;
            int lb = to_left[indptr[i] + b] - to_left[indptr[i]];
            int rb = b - lb;
            int m = (l + r) / 2;
            dfs(dfs, 2 * i + 0, l, m, la, lb);
            dfs(dfs, 2 * i + 1, m, r, ra, rb);
        };
        dfs(dfs, 1, 0, size, LB(all_Y, ly), LB(all_Y, ry));
        return res;
    }

private:
    template <typename T, typename U>
    vector<T> cumsum(vector<U> &A, int off = 1) {
        int N = A.size();
        vector<T> B(N + 1);
        for(int i = 0; i < N; i++) { B[i + 1] = B[i] + A[i]; }
        if (off == 0) B.erase(B.begin());
        return B;
    }

    inline int xtoi(XY x) {
        if constexpr (SMALL_X) return clamp<XY>(x - minX, 0, NX);
        return lower_bound(keyX.begin(), keyX.end(), x) - keyX.begin();
    }

    S prod_i(int i, int a, int b) {
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        int off = 2 * LID;
        int L = n + a, R = n + b;
        S val = MX::unit();
        while (L < R) {
            if (L & 1) val = MX::op(val, dat[off + (L++)]);
            if (R & 1) val = MX::op(dat[off + (--R)], val);
            L >>= 1, R >>= 1;
        }
        return val;
    }
    void multiply_i(int i, int j, S val) {
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        int off = 2 * LID;
        j += n;
        while (j) {
            dat[off + j] = MX::op(dat[off + j], val);
            j >>= 1;
        }
    }
    void set_i(int i, int j, S val) {
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        int off = 2 * LID;
        j += n;
        dat[off + j] = val;
        while (j > 1) {
            j /= 2;
            dat[off + j] = MX::op(dat[off + 2 * j + 0], dat[off + 2 * j + 1]);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_SEGTREE_2D