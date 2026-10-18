#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"
#include <library/template.hpp>

#include <library/string/suffix_array.hpp>

void solve() {
    STR(S);
    long long N = len(S);
    Suffix_Array sa(S);
    long long ANS = N * (N + 1) / 2;
    ANS -= SUM<long long>(sa.LCP);
    print(ANS);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
