#ifndef AJAY_SPARSE_TABLE
#define AJAY_SPARSE_TABLE

namespace mitsuha{
// Monoid must be idempotent, faster than disjoint sparse table
template <class Monoid>
struct Sparse_Table {
    using MX = Monoid;
    using X = typename MX::value_type;
    int n, log;
    vector<vector<X>> dat;

    Sparse_Table() {}
    Sparse_Table(int n) { build(n); }
    template <typename F>
    Sparse_Table(int n, F f) {
        build(n, f);
    }
    Sparse_Table(const vector<X>& v) { build(v); }

    void build(int m) {
        build(m, [](int i) -> X { return MX::unit(); });
    }
    void build(const vector<X>& v) {
        build(len(v), [&](int i) -> X { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        n = m, log = 1;
        while ((1 << log) < n) ++log;
        dat.resize(log);
        dat[0].resize(n);
        for(int i = 0; i < n; i++) dat[0][i] = f(i);

        for(int i = 0; i < log - 1; i++) {
            dat[i + 1].resize(len(dat[i]) - (1 << i));
            for(int j = 0; j < len(dat[i]) - (1 << i); j++) {
                dat[i + 1][j] = MX::op(dat[i][j], dat[i][j + (1 << i)]);
            }
        }
    }

    X prod(int L, int R) {
        if (L == R) return MX::unit();
        if (R == L + 1) return dat[0][L];
        int k = (R - L - 1 == 0 ? -1 : 31 - __builtin_clz(R - L - 1));
        return MX::op(dat[k][L], dat[k][R - (1 << k)]);
    }

    template <class F>
    int max_right(const F check, int L) {
        assert(0 <= L && L <= n && check(MX::unit()));
        if (L == n) return n;
        int ok = L, ng = n + 1;
        while (ok + 1 < ng) {
            int k = (ok + ng) / 2;
            bool bl = check(prod(L, k));
            if (bl) ok = k;
            if (!bl) ng = k;
        }
        return ok;
    }

    template <class F>
    int min_left(const F check, int R) {
        assert(0 <= R && R <= n && check(MX::unit()));
        if (R == 0) return 0;
        int ok = R, ng = -1;
        while (ng + 1 < ok) {
            int k = (ok + ng) / 2;
            bool bl = check(prod(k, R));
            if (bl) ok = k;
            if (!bl) ng = k;
        }
        return ok;
    }
};
} // namespace mitsuha
#endif // AJAY_SPARSE_TABLE