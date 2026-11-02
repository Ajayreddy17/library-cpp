#ifndef AJAY_RESTORE_EULER_TOUR
#define AJAY_RESTORE_EULER_TOUR

#include "library/datastructure/decremental_fastset.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/restore_euler_tour.hpp

// the vertex sequence of an Euler tour of an unknown tree is given, with -1 for unknowns
// https://codeforces.com/problemset/problem/1053/E
// the start is not necessarily 0
vector<int> restore_euler_tour(int N, vector<int> A) {
    if (N == 1) return {0};
    assert(len(A) == N + N - 1);
    if (A[0] == -1) A[0] = A.back();
    if (A.back() == -1) A.back() = A[0];
    if (A[0] != -1 && A.back() != -1 && A[0] != A.back()) return {};

    vector<int> exist(N);
    for (auto& x : A)
        if (0 <= x) exist[x] = 1;
    vector<int> other;
    For(x, N) if (!exist[x]) other.emplace_back(x);
    For(N + N - 1) other.emplace_back(N);
    reverse(other.begin(), other.end());

    vector<int> nxt(N + N - 1, -1), pre(N + N - 1, -1);
    {
        vector<int> pos(N, -1);
        For(i, N + N - 1) {
            int x = A[i];
            if (x == -1) continue;
            if (pos[x] != -1) nxt[pos[x]] = i, pre[i] = pos[x];
            pos[x] = i;
        }
    }

    min_priority_queue<tuple<int, int, int>> que;
    For(i, N + N - 1) if (nxt[i] != -1) que.emplace(nxt[i] - i, i, nxt[i]);
    Decremental_FastSet FS(N + N - 1);

    auto rm = [&](int i) -> void {
        assert(FS[i]);
        FS.erase(i);
        int a = pre[i], b = nxt[i];
        if (a != -1) nxt[a] = b;
        if (b != -1) pre[b] = a;
        if (a != -1 && b != -1) que.emplace(b - a, a, b);
    };

    auto solve_local = [&](vector<int>& A) -> void {
        int N = len(A);
        assert(N >= 3 && A[0] == A[N - 1] && N % 2 == 1);
        // r.....r
        // values in between are distinct and few kinds may be assumed
        int n = 0;
        For(i, 1, N - 1) n += (A[i] != -1);
        if (n > N / 2) return;
        For(i, 1, N - 1) {
            if (A[i] == -1 && n < N / 2) A[i] = POP(other), ++n;
        }

        vector<int> I;
        For(i, 1, N - 1) {
            I.emplace_back(i);
            while (len(I) >= 3) {
                int a = I[len(I) - 3];
                int b = I[len(I) - 2];
                int c = I[len(I) - 1];
                if (A[a] == -1 && A[b] != -1 && A[c] != -1) {
                    A[a] = A[c];
                    POP(I), POP(I);
                }
                elif (A[a] != -1 && A[b] != -1 && A[c] == -1) {
                    A[c] = A[a];
                    POP(I), POP(I);
                }
                else break;
            }
        }
        if (*min_element(A.begin(), A.end()) >= 0) return;
        for (auto& x : A)
            if (x == -1) x = A[0];
    };

    while (len(que)) {
        auto [pri, L, R] = POP(que);
        if (!FS[L] || !FS[R]) continue;
        vector<int> I;
        I.emplace_back(L);
        FS.enumerate(L + 1, R, [&](int i) -> void { I.emplace_back(i); });
        I.emplace_back(R);
        vector<int> B = rearrange(A, I);
        if (len(B) % 2 == 0) return {};
        solve_local(B);
        For(i, len(B)) A[I[i]] = B[i];
        FS.enumerate(L, R, [&](int i) -> void { rm(i); });
    }

    auto solve_local_2 = [&](vector<int>& A) -> void {
        int N = len(A);
        assert(N >= 3 && A[0] == A[N - 1] && N % 2 == 1 && A[0] == -1);
        if (N == 3) {
            A[0] = A[N - 1] = POP(other);
            solve_local(A);
            return;
        }
        // ?.....?
        // values in between are distinct and few kinds may be assumed
        int n = 0;
        For(i, 1, N - 1) n += (A[i] != -1);
        if (n > 1 + N / 2) return;
        if (n <= N / 2) {
            A[0] = A[N - 1] = POP(other);
            solve_local(A);
            return;
        }
        // .23.10. every kind appears
        // so roughly the same procedure works

        vector<int> I;
        For(i, 1, N - 1) {
            I.emplace_back(i);
            while (len(I) >= 3) {
                int a = I[len(I) - 3];
                int b = I[len(I) - 2];
                int c = I[len(I) - 1];
                if (A[a] == -1 && A[b] != -1 && A[c] != -1) {
                    A[a] = A[c];
                    POP(I), POP(I);
                }
                elif (A[a] != -1 && A[b] != -1 && A[c] == -1) {
                    A[c] = A[a];
                    POP(I), POP(I);
                }
                else break;
            }
        }
        assert(len(I) == 3);
        A[0] = A[N - 1] = A[I[1]];
    };

    if (A[0] == -1) {
        vector<int> I;
        FS.enumerate(0, len(A), [&](int i) -> void { I.emplace_back(i); });
        vector<int> B = rearrange(A, I);
        solve_local_2(B);
        For(i, len(I)) A[I[i]] = B[i];
    }

    // check
    if (*max_element(A.begin(), A.end()) >= N) return {};
    if (*min_element(A.begin(), A.end()) == -1) return {};
    vector<int> vis(N);
    vector<int> path = {A[0]};
    For(i, 1, len(A)) {
        int v = A[i];
        if (len(path) >= 2 && path[len(path) - 2] == v) {
            POP(path);
        } else {
            if (vis[v]) return {};
            path.emplace_back(v);
            vis[v] = 1;
        }
    }
    if (len(path) != 1) return {};
    return A;
}
} // namespace mitsuha
#endif // AJAY_RESTORE_EULER_TOUR
