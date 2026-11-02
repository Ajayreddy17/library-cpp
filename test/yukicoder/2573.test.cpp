#define PROBLEM "https://yukicoder.me/problems/no/2573"
#include "library/template.hpp"
#include "library/flow/hungarian.hpp"

void solve() {
    LL(H, W);
    vector<vector<long long>> mat(W, vector<long long>(W));
    For(i, W) {
        LL(x, y);
        --x, --y;
        For(j, W) {
            long long cost = 0;
            if (j < y - x) { cost = x + y - x - j; }
            elif (y - x <= j && j < y) { cost = x; }
            elif (y <= j) { cost = x + j - y; }
            mat[i][j] = cost;
        }
    }
    // For(i, W) print(mat[i]);
    auto [ans, match, X, Y] = hungarian<long long, true>(mat);
    print(ans);
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
