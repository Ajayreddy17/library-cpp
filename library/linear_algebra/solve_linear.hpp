#ifndef AJAY_SOLVE_LINEAR
#define AJAY_SOLVE_LINEAR

namespace mitsuha{
/*
One of the solutions is on line 0.
The basis of the solution space is entered as a row vector in rows 1 to 1.
No solution = empty
*/
template <typename T>
vector<vector<T>> solve_linear(vector<vector<T>> a, vector<T> b, int n = -1, int m = -1) {
    if (n == -1) {
        n = len(a);
        assert(n > 0);
        m = len(a[0]);
    }
    assert(n == len(a) && n == len(b));
    int rk = 0;
    For(j, m) {
        if (rk == n) break;
        For(i, rk, n) if (a[i][j] != 0) {
            swap(a[rk], a[i]);
            swap(b[rk], b[i]);
            break;
        }
        if (a[rk][j] == 0) continue;
        T c = T(1) / a[rk][j];
        for (auto&& x : a[rk]) x *= c;
        b[rk] *= c;
        For(i, n) if (i != rk) {
            T c = a[i][j];
            if (c == T(0)) continue;
            b[i] -= b[rk] * c;
            For(k, j, m) { a[i][k] -= a[rk][k] * c; }
        }
        ++rk;
    }
    For(i, rk, n) if (b[i] != 0) return {};
    vector<vector<T>> res(1, vector<T>(m));
    vector<int> pivot(m, -1);
    int p = 0;
    For(i, rk) {
        while (a[i][p] == 0) ++p;
        res[0][p] = b[i];
        pivot[p] = i;
    }
    For(j, m) if (pivot[j] == -1) {
        vector<T> x(m);
        x[j] = -1;
        For(k, j) if (pivot[k] != -1) x[k] = a[pivot[k]][j];
        res.emplace_back(x);
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_SOLVE_LINEAR