#define PROBLEM "https://yukicoder.me/problems/no/1201"
#include "library/template.hpp"
#include "library/mod/floor_sum_of_linear.hpp"

const int mod = 1'000'000'007;

void solve() {
    LL(N, M);
    vector<long long> A(N); read(A);
    vector<long long> B(M); read(B);

    int128 ANS = 0;
    for (auto&& a: A) {
        for (auto&& b: B) {
            // For(k, 1, b + 1) ANS += mint(fld(a * k, b));
            int128 x = floor_sum_of_linear<int128, long long>(1, b + 1, a, 0, b);
            ANS += x;
        }
    }
    ANS += ANS;
    ANS %= mod;
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
