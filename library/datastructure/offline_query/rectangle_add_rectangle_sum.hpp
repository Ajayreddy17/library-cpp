#ifndef AJAY_RECT_ADD_RECT_SUM
#define AJAY_RECT_ADD_RECT_SUM

// https://judge.yosupo.jp/submission/229013
namespace mitsuha{
template<typename T, typename XY>
struct Rectangle_Add_Rectangle_Sum {
    
    Rectangle_Add_Rectangle_Sum() {}

    void add_query(XY lx, XY rx, XY ly, XY ry, T z) {
        U.push_back(Update(lx, rx, ly, ry, z));
    }

    void sum_query(XY lx, XY rx, XY ly, XY ry) {
        Q.push_back(Query(lx, rx, ly, ry, Q.size()));
    }

    vector<T> calc() {
        vector<T> ans(Q.size(), 0);
        solve(ans);
        return ans;
    }

private:
    struct bit4 {
        int N;
        vector<array<T, 4>> sum;
        bit4(int N): N(N), sum(N + 1, {0, 0, 0, 0}) {}

        void apply(XY l, XY r, int lc, int rc, T z1, T z2) {
            T a = l * z1, b = r * z1, c = l * z2, d = r * z2;
            for (int i = lc + 1; i <= N; i += (i & (-i))) {
                sum[i][0] -= a; sum[i][1] += z1;
                sum[i][2] -= c; sum[i][3] += z2;
            }
            for (int i = rc + 1; i <= N; i += (i & (-i))) {
                sum[i][0] += b; sum[i][1] -= z1;
                sum[i][2] += d; sum[i][3] -= z2;
            }
        }

        pair<T, T> prod(XY r, int rc) {
            T a = 0, b = 0, c = 0, d = 0;
            for (int i = rc; i > 0; i -= (i & (-i))) {
                a += sum[i][0]; b += sum[i][1];
                c += sum[i][2]; d += sum[i][3];
            }
            return {a + (b * r), c + (d * r)};
        }

        pair<T, T> prod(XY l, XY r, int lc, int rc) {
            auto [cr, dxr] = prod(r, rc);
            auto [cl, dxl] = prod(l, lc);
            return {cr - cl, dxr - dxl};
        }
    };

    struct Update {
        XY lx, rx, ly, ry;
        T z;
        int lyc, ryc;
        Update(XY lx, XY rx, XY ly, XY ry, T z, int lyc = 0, int ryc = 0) : lx(lx), rx(rx), ly(ly), ry(ry), z(z), lyc(lyc), ryc(ryc) {}
    };

    struct Query {
        XY lx, rx, ly, ry;
        int id, lyc, ryc;
        Query(XY lx, XY rx, XY ly, XY ry, int id, int lyc = 0, int ryc = 0): lx(lx), rx(rx), ly(ly), ry(ry), id(id), lyc(lyc), ryc(ryc){}
    };

    vector<Query> Q;
    vector<Update> U;

    void solve(vector<T> &ans) {
        int N = U.size(), M = Q.size();
        vector<XY> Y;
        for (int i = 0; i < N; i++) {
            Y.push_back(U[i].ly);
            Y.push_back(U[i].ry);
        }
        settify(Y);
        auto lb = [&](XY y) -> int { return lower_bound(Y.begin(), Y.end(), y) - Y.begin(); };
        for (int i = 0; i < N; i++) {
            int lyc = lb(U[i].ly), ryc = lb(U[i].ry);
            U[i].lyc = lyc, U[i].ryc = ryc;
            U.push_back(Update(U[i].rx, 0, U[i].ly, U[i].ry, -U[i].z, lyc, ryc));
        }

        for (int i = 0; i < M; i++) {
            int lyc = lb(Q[i].ly), ryc = lb(Q[i].ry);
            Q[i].lyc = lyc, Q[i].ryc = ryc;
            Q.push_back(Query(Q[i].rx, 0, Q[i].ly, Q[i].ry, Q[i].id + M, lyc, ryc));
        }
        sort(U.begin(), U.end(), [](const Update &a, const Update &b){return a.lx < b.lx;});
        sort(Q.begin(), Q.end(), [](const Query &a, const Query &b){return a.lx < b.lx;});
        assert(U.size() == 2 * N && Q.size() == 2 * M);
        bit4 bit(Y.size());
        int uid = 0, qid = 0;

        while (qid < 2 * M) {
            if (uid < 2 * N && U[uid].lx < Q[qid].lx) {
                bit.apply(U[uid].ly, U[uid].ry, U[uid].lyc, U[uid].ryc, -U[uid].z * U[uid].lx, U[uid].z);
                uid++;
            } else {
                auto [a, b] = bit.prod(Q[qid].ly, Q[qid].ry, Q[qid].lyc, Q[qid].ryc);
                int id = Q[qid].id;
                if (id >= M) {
                    ans[id - M] += a + Q[qid].lx * b;
                } else {
                    ans[id    ] -= a + Q[qid].lx * b;
                }
                qid++;
            }
        }
    }
};
} // namespace mitsuha
#endif // AJAY_RECT_ADD_RECT_SUM