#ifndef AJAY_MATRIX_RANK
#define AJAY_MATRIX_RANK

namespace mitsuha{
template <typename T>
int matrix_rank(vector<vector<T>> a, int n = -1, int m = -1) {
    if (n == 0) return 0;
    if (n == -1) {
        n = int(a.size()), m = int(a[0].size());
    }
    assert(n == int(a.size()) && m == int(a[0].size()));
    int rk = 0;
    For(j, m) {
        if (rk == n) break;
        if (a[rk][j] == 0) {
            For(i, rk + 1, n) if (a[i][j] != T(0)) {
                swap(a[rk], a[i]);
                break;
            }
        }
        if (a[rk][j] == 0) continue;
        T c = T(1) / a[rk][j];
        For(k, j, m) a[rk][k] *= c;
        For(i, rk + 1, n) {
            T c = a[i][j];
            For(k, j, m) a[i][k] -= a[rk][k] * c;
        }
        ++rk;
    }
    return rk;
}
} // namespace mitsuha
#endif // AJAY_MATRIX_RANK