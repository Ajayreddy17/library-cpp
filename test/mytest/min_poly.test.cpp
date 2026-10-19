#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/linear_algebra/blackbox/min_poly.hpp>
#include <library/linear_algebra/matrix_mul.hpp>

using mint = modint998;

void test() {
    vector<tuple<int, int, mint>> A;
    A.emplace_back(0, 0, 1);
    A.emplace_back(0, 1, 2);
    A.emplace_back(1, 0, 3);
    A.emplace_back(1, 1, 4);
    vector<mint> f = blackbox_min_poly<mint>(2, [&](vector<mint> f) -> vector<mint> {
        vector<mint> g(2);
        for (auto& [a, b, c]: A) g[b] += f[a] * c;
        return g;
    });
    assert(f == vector<mint>({mint(-2), mint(-5), mint(1)}));

    A.clear();
    A.emplace_back(0, 1, 1);
    f = blackbox_min_poly<mint>(2, [&](vector<mint> f) -> vector<mint> {
        vector<mint> g(2);
        for (auto& [a, b, c]: A) g[b] += f[a] * c;
        return g;
    });
    assert(f == vector<mint>({mint(0), mint(0), mint(1)}));

    A.clear();
    f = blackbox_min_poly<mint>(2, [&](vector<mint> f) -> vector<mint> {
        vector<mint> g(2);
        for (auto& [a, b, c]: A) g[b] += f[a] * c;
        return g;
    });
    assert(f == vector<mint>({mint(0), mint(1)}));

    A.clear();
    f = blackbox_min_poly<mint>(0, [&](vector<mint> f) -> vector<mint> {
        vector<mint> g(0);
        for (auto& [a, b, c]: A) g[b] += f[a] * c;
        return g;
    });
    assert(f == vector<mint>({mint(1)}));

    A.clear();
    A.emplace_back(0, 1, 1);
    A.emplace_back(0, 2, 1);
    A.emplace_back(1, 2, 1);
    f = blackbox_min_poly<mint>(3, [&](vector<mint> f) -> vector<mint> {
        vector<mint> g(3);
        for (auto& [a, b, c]: A) g[b] += f[a] * c;
        return g;
    });
    assert(f == vector<mint>({mint(0), mint(0), mint(0), mint(1)}));

    A.clear();
    f = blackbox_min_poly<mint>(3, [&](vector<mint> a) -> vector<mint> {
        vector<mint> b(3);
        for (auto&& [i, j, x]: A) b[j] += a[i] * x;
        return b;
    });
    assert(f == vector<mint>({mint(0), mint(1)}));

    // random matrix
    For(N, 1, 20) {
        vector<vector<mint>> mat(N, vector<mint>(N));
        For(i, N) For(j, N) mat[i][j] = RNG(0, 998244353);
        A.clear();
        For(i, N) For(j, N) A.emplace_back(i, j, mat[i][j]);
        f = blackbox_min_poly<mint>(N, [&](vector<mint> a) -> vector<mint> {
            vector<mint> b(N);
            for (auto&& [i, j, x]: A) b[j] += a[i] * x;
            return b;
        });
        vector<vector<mint>> B(N, vector<mint>(N));
        For(i, N) B[i][i] = 1;
        vector<vector<mint>> C(N, vector<mint>(N));
        For(d, len(f)) {
            For(i, N) For(j, N) C[i][j] += f[d] * B[i][j];
            B = matrix_mul(mat, B);
        }
        For(i, N) For(j, N) assert(C[i][j] == mint(0));
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