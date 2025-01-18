#ifndef AJAY_MAXIMAL_SQUARE
#define AJAY_MAXIMAL_SQUARE

namespace mitsuha{
// Returns { max_area, { u, d, l, r } } where g[u,d)*[l,r) is one of the maximum squares.
template <typename Grid>
pair<int, tuple<int, int, int, int>> maximum_square(const Grid& g) {
    int max_l = 0;
    tuple<int, int, int, int> max_square;

    const int n = g.size();
    const int m = n == 0 ? 0 : g[0].size();
        
    vector<int> pd(m);
    for (int i = 0; i < n; ++i) {
        vector<int> dp(m);
        for (int j = 0; j < m; ++j) if (g[i][j]) {
            if (j == 0) {
                dp[j] = 1;
            } else {
                dp[j] = 1 + min({ dp[j - 1], pd[j - 1], pd[j] });
            }
            if (dp[j] > max_l) {
                max_l = dp[j];
                max_square = { i + 1 - max_l, i + 1, j + 1 - max_l, j + 1 };
            }
        }
        dp.swap(pd);
    }
    return { max_l * max_l, max_square };
}
} // namespace 
#endif // AJAY_MAXIMAL_SQUARE