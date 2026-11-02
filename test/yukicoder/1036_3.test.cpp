#define PROBLEM "https://yukicoder.me/problems/no/1036"
#include "library/template.hpp"
#include "library/datastructure/sliding_window_aggregation.hpp"
#include "library/monoid/monoid_gcd.hpp"

void solve() {
    LL(N);
    vector<long long> A(N); read(A);
    Sliding_Window_Aggregation<Monoid_Gcd<long long>> swag;
    long long R = 0;
    long long ANS = 0;
    For(L, N) {
        chmax(R, L);
        while (R < N && gcd(swag.prod(), A[R]) != 1) swag.push(A[R++]);
        ANS += N - R;
        if (len(swag)) swag.pop();
    }
    print(ANS);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    long long T = 1;
    // LL(T);
    For(_, T) solve();

    return 0;
}
