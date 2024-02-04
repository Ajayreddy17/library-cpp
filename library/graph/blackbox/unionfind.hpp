#ifndef AJAY_BLACKBOX_UNIONFIND
#define AJAY_BLACKBOX_UNIONFIND

#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// Find the next edge using a suitable data structure while deleting vertices.
// Since the contents are just bfs, it can also be used for 01 Shortest Path
template <typename F1, typename F2>
UnionFind blackbox_unionfind(int N, F1 set_used, F2 find_unused) {
    UnionFind uf(N);
    vector<bool> done(N);
    deque<int> que;
    For(v, N) if (!done[v]) {
        que.emplace_back(v);
        done[v] = 1;
        set_used(v);
        while (!que.empty()) {
            int x = que.front();
            que.pop_front();
            set_used(x);
            done[x] = 1;
            while (true) {
                int to = find_unused(x);
                if (to == -1) break;
                uf.merge(v, to);
                que.emplace_back(to);
                done[to] = 1;
                set_used(to);
            }
        }
    }
    return uf;
}
} // namespace mitsuha
#endif // AJAY_BLACKBOX_UNIONFIND