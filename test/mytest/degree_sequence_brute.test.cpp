#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/degree_sequence.hpp"

// every sequence with N <= 6 against all simple graphs on N vertices
void test() {
    For(N, 7) {
        int E = N * (N - 1) / 2;
        vector<pair<int, int>> all;
        For(i, N) For(j, i + 1, N) all.emplace_back(i, j);
        set<vector<int>> graphical;
        For(m, 1 << E) {
            vector<int> d(N);
            For(k, E) if (m >> k & 1) d[all[k].first]++, d[all[k].second]++;
            graphical.insert(d);
        }
        int tot = 1;
        For(N) tot *= N + 1;
        For(c, tot) {
            vector<int> d(N);
            int x = c;
            For(i, N) d[i] = x % (N + 1), x /= N + 1;
            bool exp = graphical.count(d);
            assert(check_degree_sequence(d) == exp);
            if (!exp) continue;
            vector<int> dd(N);
            set<pair<int, int>> seen;
            for (auto [a, b]: construct_from_degree_sequence(d)) {
                assert(a != b && seen.insert({min(a, b), max(a, b)}).second);
                dd[a]++, dd[b]++;
            }
            assert(dd == d);
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test();
    solve();
    return 0;
}
