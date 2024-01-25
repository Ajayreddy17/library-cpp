#ifndef AJAY_DYNAMIC_LICHAO
#define AJAY_DYNAMIC_LICHAO

namespace mitsuha{
// Space Q is sufficient if it is straight line addition and non-persistent.
// Function is ll -> T. [L, R) Above Be careful not to overflow f.
// If you rewrite evaluate, it can also be used for a group of totally monotone functions
template <typename T, bool PERSISTENT, int NODES, bool MINIMIZE>
struct Dynamic_LiChao_Tree {
    using FUNC = pair<T, T>;
    vector<FUNC> funcs;

    static inline T evaluate(FUNC &f, long long x) { return f.first * x + f.second; }

    struct Node {
        int fid;
        Node *l, *r;
    };

    Node *pool;
    int pid;
    long long L, R;

    using np = Node *;

    Dynamic_LiChao_Tree(long long L, long long R) : pid(0), L(L), R(R) {
        pool = new Node[NODES];
    }

    void reset() {
        funcs.clear();
        pid = 0;
    }

    np new_root() { return nullptr; }

    np new_node() {
        pool[pid].fid = -1;
        pool[pid].l = nullptr, pool[pid].r = nullptr;
        return &(pool[pid++]);
    }

    np add_line(np root, FUNC f) {
        int fid = len(funcs);
        funcs.emplace_back(f);
        if (!root) root = new_node();
        return add_line_rec(root, fid, L, R);
    }

    // [xl, xr)
    np add_segment(np root, long long xl, long long xr, FUNC f) {
        int fid = len(funcs);
        funcs.emplace_back(f);
        if (!root) root = new_node();
        return add_segment_rec(root, xl, xr, fid, L, R);
    }

    // (value/function number)
    pair<T, int> query(np root, long long x) {
        assert(L <= x && x < R);
        if (!root) {
            if (MINIMIZE) return {numeric_limits<T>::max() / 2, -1};
            if (!MINIMIZE) return {numeric_limits<T>::min() / 2, -1};
        }
        return query_rec(root, x, L, R);
    }

private:
    np copy_node(Node *c) {
        if (!c || !PERSISTENT) return c;
        pool[pid].fid = c->fid;
        pool[pid].l = c->l, pool[pid].r = c->r;
        return &(pool[pid++]);
    }

    inline T evaluate_inner(int fid, long long x) {
        if (fid == -1) { return (MINIMIZE ? numeric_limits<T>::max() / 2 : numeric_limits<T>::min() / 2); };
        return evaluate(funcs[fid], x);
    }

    np add_segment_rec(np c, long long xl, long long xr, int fid, long long node_l, long long node_r) {
        chmax(xl, node_l), chmin(xr, node_r);
        if (xl >= xr) return c;
        if (node_l < xl || xr < node_r) {
            c = copy_node(c);
            long long node_m = (node_l + node_r) / 2;
            if (!c->l) c->l = new_node();
            if (!c->r) c->r = new_node();
            c->l = add_segment_rec(c->l, xl, xr, fid, node_l, node_m);
            c->r = add_segment_rec(c->r, xl, xr, fid, node_m, node_r);
            return c;
        }
        return add_line_rec(c, fid, node_l, node_r);
    }

    np add_line_rec(np c, int fid, long long node_l, long long node_r) {
        int gid = c->fid;
        T fl = evaluate_inner(fid, node_l), fr = evaluate_inner(fid, node_r - 1);
        T gl = evaluate_inner(gid, node_l), gr = evaluate_inner(gid, node_r - 1);
        bool bl = (MINIMIZE ? fl < gl : fl > gl);
        bool br = (MINIMIZE ? fr < gr : fr > gr);
        if (bl && br) {
            c = copy_node(c);
            c->fid = fid;
            return c;
        }
        if (!bl && !br) { return c; }

        c = copy_node(c);
        long long node_m = (node_l + node_r) / 2;
        auto fm = evaluate_inner(fid, node_m), gm = evaluate_inner(gid, node_m);
        bool bm = (MINIMIZE ? fm < gm : fm > gm);
        if (bm) {
            c->fid = fid;
            if (bl) {
                if (!c->r) c->r = new_node();
                c->r = add_line_rec(c->r, gid, node_m, node_r);
            } else {
                if (!c->l) c->l = new_node();
                c->l = add_line_rec(c->l, gid, node_l, node_m);
            }
        }
        if (!bm) {
            if (!bl) {
                if (!c->r) c->r = new_node();
                c->r = add_line_rec(c->r, fid, node_m, node_r);
            } else {
                if (!c->l) c->l = new_node();
                c->l = add_line_rec(c->l, fid, node_l, node_m);
            }
        }
        return c;
    }

    pair<T, int> query_rec(np c, long long x, long long node_l, long long node_r) {
        int fid = c->fid;
        pair<T, int> res = {evaluate_inner(fid, x), fid};
        long long node_m = (node_l + node_r) / 2;
        if (x < node_m && c->l) {
            pair<T, int> res1 = query_rec(c->l, x, node_l, node_m);
            res = (MINIMIZE ? min(res, res1) : max(res, res1));
        }
        if (x >= node_m && c->r) {
            pair<T, int> res1 = query_rec(c->r, x, node_m, node_r);
            res = (MINIMIZE ? min(res, res1) : max(res, res1));
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_LICHAO