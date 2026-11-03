#ifndef AJAY_STABLE_ROOMMATES
#define AJAY_STABLE_ROOMMATES

#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/matching/stable_roommates.hpp

// https://codeforces.com/contest/1423/problem/A
// https://www.sciencedirect.com/science/article/abs/pii/0196677485900331?via%3Dihub
// preference lists: earlier entries are preferred
// pad with INF if needed so every list has length N-1
vector<pair<int, int>> stable_roommates(vector<vector<int>> prefer_list) {
    vector<vector<int>>& A = prefer_list;
    int N = len(A);
    For(i, N - 1) assert(len(A[i]) == N - 1);

    For(v, N) debug(v, A[v]);

    vector<vector<int>> rank(N, vector<int>(N));
    For(i, N) For(j, N - 1) rank[i][A[i][j]] = j;

    vector<vector<bool>> exist(N, vector<bool>(N, 1));
    For(i, N) exist[i][i] = 0;

    vector<int> que(N);  // people who have not proposed
    For(i, N) que[i] = i;
    vector<int> L(N), R(N, N - 1);
    vector<int> TO(N, -1), FRM(N, -1);

    auto clip = [&](int i) -> void {
        while (L[i] < R[i] && !exist[i][A[i][L[i]]]) ++L[i];
        while (L[i] < R[i] && !exist[i][A[i][R[i] - 1]]) --R[i];
    };

    auto nxt = [&](int i) -> int {
        // second
        clip(i);
        assert(R[i] - L[i] >= 2);
        int a = A[i][L[i]++];
        clip(i);
        A[i][--L[i]] = a;
        return FRM[A[i][L[i] + 1]];
    };

    while (1) {
        while (len(que)) {
            int x = POP(que);
            assert(TO[x] == -1);
            while (1) {
                clip(x);
                if (L[x] == R[x]) return {};
                int y = A[x][L[x]];
                if (FRM[y] != -1 && rank[y][x] > rank[y][FRM[y]]) {
                    exist[x][y] = exist[y][x] = 0, ++L[x];
                    continue;
                }
                if (FRM[y] != -1) {
                    que.emplace_back(FRM[y]), exist[FRM[y]][y] = exist[y][FRM[y]] = 0;
                    TO[FRM[y]] = -1, FRM[y] = -1;
                }
                TO[x] = y, FRM[y] = x;
                while (A[y][R[y] - 1] != x) {
                    int z = A[y][--R[y]];
                    exist[y][z] = exist[z][y] = 0;
                }
                break;
            }
        }
        debug(FRM, TO);
        For(v, N) debug(v, A[v], L[v], R[v]);
        For(v, N) debug(exist[v]);
        int s = [&]() -> int {
            For(s, N) {
                clip(s);
                if (R[s] - L[s] > 1) return s;
            }
            return -1;
        }();
        if (s == -1) break;
        int x = s, y = s;
        do {
            x = nxt(x), y = nxt(nxt(y));
        } while (x != y);
        vector<int> cyc;
        do {
            cyc.emplace_back(y), y = nxt(y);
        } while (x != y);
        for (int x : cyc) {
            int y = TO[x];
            FRM[y] = -1, TO[x] = -1, exist[x][y] = exist[y][x] = 0, que.emplace_back(x);
        }
    }
    vector<pair<int, int>> ANS;
    For(i, N) {
        int j = TO[i];
        assert(TO[j] == i);
        if (i < j) ANS.emplace_back(i, j);
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_STABLE_ROOMMATES
