#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/random/base.hpp>
#include <library/string/is_substring.hpp>

string gen(int n) {
    string s;
    For(n) { s += char('a' + RNG(3)); }
    return s;
}

bool naive(string S, string T) {
    For(i, len(T)) if (T.substr(i, len(S)) == S) return true;
    return false;
}

void test() {
    For(1000) {
        For(n, 1, 10) For(m, 1, 10) {
            string s = gen(n), t = gen(m);
            bool a = naive(s, t);
            bool b = is_substring(s, t);
            assert(a == b);
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