#ifndef AJAY_MO_ON_TREE
#define AJAY_MO_ON_TREE

#include "library/graph/tree.hpp"
#include "library/datastructure/offline_query/mo.hpp"

namespace mitsuha{
// https://codeforces.com/contest/852/problem/I
template <typename TREE, bool ORIENTED = false>
struct Mo_on_Tree {
    TREE& tree;
    vector<pair<int, int>> LR;

    Mo mo;
    Mo_on_Tree(TREE& tree) : tree(tree) {}
    void add(int u, int v) {
        if constexpr (!ORIENTED) {
            if (tree.LID[u] > tree.LID[v]) swap(u, v);
        }
        LR.emplace_back(tree.ELID(u) + 1, tree.ELID(v) + 1);
    }

    // init(): root consisting only of path
    // add_l(v), add_r(v)：Add v to beginning/end of path
    // rm_l(v), rm_r(v)：Remove v from beginning/end of path
    // query(qid)
    template <typename F1, typename F2, typename F3, typename F4, typename F5, typename F6>
    void calc_vertex(F1 init, F2 add_l, F3 add_r, F4 rm_l, F5 rm_r, F6 query) {
        const int N = tree.G.N;
        auto I = Mo::get_mo_order(LR);

        vector<int> FRM(2 * N), TO(2 * N), idx(2 * N);
        vector<int> cnt(N);
        deque<int> path = {0};
        For(v, N) {
            int a = tree.ELID(v), b = tree.ERID(v);
            FRM[a] = tree.parent[v], TO[a] = v;
            FRM[b] = v, TO[b] = tree.parent[v];
            idx[a] = idx[b] = v;
        }

        auto flip_left = [&](int i) -> void {
            const int a = FRM[i], b = TO[i], c = idx[i];
            if (cnt[c] == 0) {
                int v = path.front() ^ a ^ b;
                path.emplace_front(v), add_l(v);
            } else {
                int v = path.front();
                path.pop_front(), rm_l(v);
            }
            cnt[c] ^= 1;
        };
        auto flip_right = [&](int i) -> void {
            const int a = FRM[i], b = TO[i], c = idx[i];
            if (cnt[c] == 0) {
                int v = path.back() ^ a ^ b;
                path.emplace_back(v), add_r(v);
            } else {
                int v = path.back();
                path.pop_back(), rm_r(v);
            }
            cnt[c] ^= 1;
        };

        init();

        int l = 1, r = 1;
        for (auto idx: I) {
            int L = LR[idx].fi, R = LR[idx].se;
            while (l > L) { flip_left(--l); }
            while (r < R) { flip_right(r++); }
            while (l < L) { flip_left(l++); }
            while (r > R) { flip_right(--r); }
            query(idx);
        }
    }

    // init(): root consisting only of path
    // add_l(frm, to), add_r(frm, to)：Add (frm,to) to beginning/end of path
    // rm_l(frm, to), rm_r(frm, to)：Remove (frm,to) to beginning/end of path
    // query(qid)
    template <typename F1, typename F2, typename F3, typename F4, typename F5, typename F6>
    void calc_edge(F1 init, F2 add_l, F3 add_r, F4 rm_l, F5 rm_r, F6 query) {
        const int N = tree.G.N;
        auto I = Mo::get_mo_order(LR);

        vector<int> FRM(2 * N), TO(2 * N), idx(2 * N);
        vector<int> cnt(N);
        deque<int> path = {0};
        For(v, N) {
            int a = tree.ELID(v), b = tree.ERID(v);
            FRM[a] = tree.parent[v], TO[a] = v;
            FRM[b] = v, TO[b] = tree.parent[v];
            idx[a] = idx[b] = v;
        }

        auto flip_left = [&](int i) -> void {
            const int a = FRM[i], b = TO[i], c = idx[i];
            if (cnt[c] == 0) {
                int v = path.front() ^ a ^ b;
                path.emplace_front(v), add_l(v, v ^ a ^ b);
            } else {
                int v = path.front();
                path.pop_front(), rm_l(v, v ^ a ^ b);
            }
            cnt[c] ^= 1;
        };
        auto flip_right = [&](int i) -> void {
            const int a = FRM[i], b = TO[i], c = idx[i];
            if (cnt[c] == 0) {
                int v = path.back() ^ a ^ b;
                path.emplace_back(v), add_r(v ^ a ^ b, v);
            } else {
                int v = path.back();
                path.pop_back(), rm_r(v ^ a ^ b, v);
            }
            cnt[c] ^= 1;
        };

        init();

        int l = 1, r = 1;
        for (auto idx: I) {
            int L = LR[idx].fi, R = LR[idx].se;
            while (l > L) { flip_left(--l); }
            while (r < R) { flip_right(r++); }
            while (l < L) { flip_left(l++); }
            while (r > R) { flip_right(--r); }
            query(idx);
        }
    }
};
} // namespace mitsuha
#endif // AJAY_MO_ON_TREE