#define PROBLEM "https://yukicoder.me/problems/no/899"
#include "library/template.hpp"
#include "library/graph/datastructure/bfs_numbering.hpp"
#include "library/datastructure/segment_tree/lazy_segtree.hpp"
#include "library/acted_monoid/am_sum_mul.hpp"

void solve() {
    LL(N);
    Graph<int> G(N);
    G.read_tree(0, 0);

    BFS_Numbering<decltype(G)> BFS(G);
    auto &ID = BFS.ID;
    vector<long long> seg_raw(N);

    For(v, N) {
        LL(a);
        seg_raw[ID[v]] = a;
    }

    using AM = ActedMonoid_Sum_Mul<long long>;
    Lazy_SegTree<AM> seg(seg_raw);

    LL(Q);
    For(_, Q) {
        LL(v);
        long long p = BFS.parent[v];
        long long pp = (p == -1 ? -1 : BFS.parent[p]);
        long long x = 0;
        if (pp >= 0) x += seg.get(ID[pp]), seg.set(ID[pp], 0);
        if (p >= 0) {
            x += seg.get(ID[p]), seg.set(ID[p], 0);
            auto [l, r] = BFS.calc_range(p, BFS.depth[p] + 1);
            x += seg.prod(l, r), seg.apply(l, r, 0);
        }
        For(d, 3) {
            auto [l, r] = BFS.calc_range(v, BFS.depth[v] + d);
            x += seg.prod(l, r), seg.apply(l, r, 0);
        }
        print(x);
        seg.set(ID[v], x);
    }
}

signed main() {
    solve();
    return 0;
}
