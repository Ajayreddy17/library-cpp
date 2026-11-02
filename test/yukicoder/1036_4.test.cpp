#define PROBLEM "https://yukicoder.me/problems/no/1036"
#include "library/template.hpp"
#include "library/datastructure/sparse_table/sparse_table.hpp"
#include "library/monoid/monoid_gcd.hpp"

void solve() {
    LL(N);
    vector<long long> A(N); read(A);
    Sparse_Table<Monoid_Gcd<long long>> seg(A);
    long long R = 0;
    long long ANS = 0;
    For(L, N) {
        while (R < N && seg.prod(L, R + 1) != 1) ++R;
        ANS += N - R;
    }
    print(ANS);
}

signed main() {
    solve();

    return 0;
}
