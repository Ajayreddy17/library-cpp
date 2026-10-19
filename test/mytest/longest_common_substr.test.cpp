#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/random/base.hpp>
#include <library/string/longest_common_substring.hpp>

void test_pair(string A, string B) {
    auto [a1, a2, b1, b2] = longest_common_substring<string>(A, B);
    {
        assert(0 <= a1 && a1 <= a2 && a2 <= len(A));
        assert(0 <= b1 && b1 <= b2 && b2 <= len(B));
        string x = A.substr(a1, a2 - a1);
        string y = B.substr(b1, b2 - b1);
        assert(x == y);
    }
    int n = a2 - a1 + 1;
    set<string> ss;
    For(i, len(A) - n + 1) { ss.insert(A.substr(i, n)); }
    For(i, len(B) - n + 1) { assert(!ss.count(B.substr(i, n))); }
}

void test() {
    For(n, 1, 20) For(m, 1, 20) {
        For(100) {
            string s, t;
            For(n) {
                int x = RNG(0, 3);
                s += 'a' + x;
            }
            For(m) {
                int x = RNG(0, 3);
                t += 'a' + x;
            }
            test_pair(s, t);
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
