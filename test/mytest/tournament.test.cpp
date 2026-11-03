#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/connectivity/tournament.hpp"

void test() {
    auto gen = [&](int N) -> vector<vector<bool>> {
        vector<vector<bool>> mat(N, vector<bool>(N));
        For(i, N) For(j, i) {
            bool b = RNG(0, 2);
            if (b) mat[i][j] = 1;
            if (!b) mat[j][i] = 1;
        }
        return mat;
    };

    For(10) {
        For(N, 1, 20) {
            auto G = gen(N);
            auto check = [&](int i, int j) -> bool { return G[i][j]; };
            auto P = hamiltonian_path_in_tournament(N, check);
            vector<bool> use(N);
            for (auto&& x: P) use[x] = 1;
            assert(len(P) == N);
            assert(accumulate(use.begin(), use.end(), int(0)) == N);
            For(i, N - 1) {
                long long a = P[i], b = P[i + 1];
                assert(G[a][b]);
            }
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test();
    solve();

    return 0;
}
