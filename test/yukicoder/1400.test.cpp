#define PROBLEM "https://yukicoder.me/problems/no/1400"
#include "library/template.hpp"
#include "library/datastructure/bit_array.hpp"
#include "library/graph/bitset/bfs_bitset.hpp"

using BS = Bit_Array;

void solve() {
    LL(N, T);
    vector<BS> G(N + N, BS(N + N));
    For(i, N) {
        STR(S);
        For(j, N) {
            if (S[j] == '0') continue;
            G[i][j + N] = 1;
            G[i + N][j] = 1;
        }
    }

    if (N == 1 && G[0][1] == 0) return print(Yes[!(1)]);

    For(s, N) {
        auto dist = bfs_bitset<BS>(G, s);
        if (T % 2 == 0) {
            For(t, N) {
                if (dist[t] == infty<int> || dist[t] > T) return print(Yes[!(1)]);
            }
        }
        if (T % 2 == 1) {
            For(t, N, N + N) {
                if (dist[t] == infty<int> || dist[t] > T) return print(Yes[!(1)]);
            }
        }
    }
    print(Yes[1]);
}

signed main() {
    solve();
    return 0;
}
