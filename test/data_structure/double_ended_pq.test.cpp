#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"
#include <library/template.hpp>
#include <library/datastructure/removable_queue.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    Removable_Queue<min_priority_queue<int>> que1(A);
    Removable_Queue<max_priority_queue<int>> que2(A);

    For(Q) {
        LL(t);
        if (t == 0) {
            LL(x);
            que1.push(x), que2.push(x);
        }
        if (t == 1) {
            int x = que1.pop();
            print(x), que2.remove(x);
        }
        if (t == 2) {
            int x = que2.pop();
            print(x), que1.remove(x);
        }
    }
}

signed main() {
    solve();
    return 0;
}
