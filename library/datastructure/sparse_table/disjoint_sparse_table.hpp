#ifndef AJAY_DISJOINT_SPARSE_TABLE
#define AJAY_DISJOINT_SPARSE_TABLE

namespace mitsuha{
template <class Monoid>
struct Disjoint_Sparse_Table {
    using MX = Monoid;
    using X = typename MX::value_type;
    int n, log;
    vector<vector<X>> dat;

    Disjoint_Sparse_Table() {}
    Disjoint_Sparse_Table(int n) { build(n); }
    template <typename F>
    Disjoint_Sparse_Table(int n, F f) {
        build(n, f);
    }
    Disjoint_Sparse_Table(const vector<X>& v) { build(v); }

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
        dat[0].reserve(n);
        for(int i = 0; i < n; i++) dat[0].emplace_back(f(i));
        for(int i = 1; i < log; i++) {
            auto& v = dat[i];
            v = dat[0];
            int b = 1 << i;
            for (int m = b; m <= n; m += 2 * b) {
                int L = m - b, R = min(n, m + b);
                for(int j = m - 1; j >= L + 1; j--) v[j - 1] = MX::op(v[j - 1], v[j]);
                for(int j = m; j < R - 1; j++) v[j + 1] = MX::op(v[j], v[j + 1]);
            }
        }
    }

    X prod(int L, int R) {
        if (L == R) return MX::unit();
        --R;
        if (L == R) return dat[0][L];
        int k = ((L ^ R) == 0 ? -1 : 31 - __builtin_clz(L ^ R));
        return MX::op(dat[k][L], dat[k][R]);
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
#endif // AJAY_DISJOINT_SPARSE_TABLE