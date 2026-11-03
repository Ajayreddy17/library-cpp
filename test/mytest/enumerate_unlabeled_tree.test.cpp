#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/graph/tree/subtree_hash.hpp"
#include "library/enumerate/unlabeled_tree.hpp"

void test(int n) {
    /*
    - the counts are correct
    - each result is a tree
    - the rest is checked by eye
    */
    int cnt = 0;
    enumerate_unlabeled_tree(n, [&](vector<pair<int, int>> edges) -> void {
        ++cnt;
        assert(len(edges) == n - 1);
        // if (n <= 6) { print(n, ",", edges); }
        UnionFind uf(n);
        for (auto& [a, b]: edges) { assert(uf.merge(a, b)); }
    });
    // https://oeis.org/A000055
    vector<long long> AC = {
            0, 1, 1, 1, 2, 3, 6, 11, 23, 47, 106, 235, 551, 1301, 3159, 7741, 19320, 48629, 123867, 317955, 823065, 2144505, 5623756, 14828074, 39299897, 104636890,
    };
    assert(cnt == AC[n]);
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    For(n, 1, 21) test(n);
    solve();
    return 0;
}
