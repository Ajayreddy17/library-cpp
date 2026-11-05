#define PROBLEM "https://yukicoder.me/problems/no/2122"
#include "library/template.hpp"
#include "library/graph/cycle/functional.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/coef_of_rational_fps.hpp"

using mint = modint<10000>;

long long to(long long n, long long M) {
    vector<mint> f = {0, 1};
    vector<mint> g = {1, -n, -1};
    mint x = coef_of_rational_fps<mint>(f, g, M);
    if (M % 2 == 1) x -= mint(1);
    return x.val;
}

void test() {
    using Re = long double;
    For(n, 10) {
        Re sqD = sqrt(n * n + 4);
        Re a = (n + sqD) / 2;
        For(M, 7) {
            Re x = 1.0;
            For(M) x *= a;
            x /= sqD;
            int v = floor(x);
            v %= 10000;
            assert(v == to(n, M));
        }
    }
}

void solve() {
    test();
    STR(S);
    LL(M, L);

    long long N = 10000;
    FunctionalGraph<> X(N);
    For(n, N) X.add(n, to(n, M));

    long long a = stoi(S);
    X.build();
    long long ANS = X.jump(a, L).first;
    string s = to_string(ANS);
    while (len(s) < 4) s = "0" + s;
    print(s);
}

signed main() {
    solve();
    return 0;
}
