#ifndef AJAY_DYNAMIC_SEGTREE_SPARSE
#define AJAY_DYNAMIC_SEGTREE_SPARSE

namespace mitsuha{
// Dynamic seg tree where most elements are always guaranteed to be unit
// Therefore, default_prod cannot be used, and acted monoid cannot be handled in general.
// The advantage is that the number of nodes can be kept to O(N) when not persisted
template <typename Monoid, bool PERSISTENT = false, int NODES = 1 << 20>
struct Dynamic_SegTree_Sparse {
    using MX = Monoid;
    using X = typename MX::value_type;

    struct Node {
        long long idx;
        Node *l, *r;
        X prod, x;
    };

    const long long L0, R0;
    Node *pool;
    int pid;
    using np = Node *;
    vector<np> FREE;

    Dynamic_SegTree_Sparse(long long L0 = -linf, long long R0 = linf) : L0(L0), R0(R0), pid(0) {
        pool = new Node[NODES];
    }

    // MLE may be avoided if used when merging tree dp etc.
    // https://codeforces.com/problemset/problem/671/D
    void free_subtree(np c) {
        auto dfs = [&](auto &dfs, np c) -> void {
            if (c->l) dfs(dfs, c->l);
            if (c->r) dfs(dfs, c->r);
            FREE.emplace_back(c);
        };
        dfs(dfs, c);
    }

    np new_root() { return nullptr; }

    np new_node(long long idx, const X x) {
        if (!FREE.empty()) {
            np c = FREE.back();
            FREE.pop_back();
            c->idx = idx, c->l = c->r = nullptr;
            c->prod = c->x = x;
            return c;
        }
        pool[pid].idx = idx;
        pool[pid].l = pool[pid].r = nullptr;
        pool[pid].x = pool[pid].prod = x;
        return &(pool[pid++]);
    }

    X operator()(np root, long long l, long long r) {
        assert(L0 <= l && l <= r && r <= R0);
        if (l == r) return MX::unit();
        X x = MX::unit();
        prod_rec(root, L0, R0, l, r, x);
        return x;
    }
    X prod(np root, long long l, long long r) {
        return (*this)(root, l, r);
    }

    X prod_all(np root) { return prod(root, L0, R0); }

    np set(np root, long long i, const X &x) {
        assert(L0 <= i && i < R0);
        return set_rec(root, L0, R0, i, x);
    }

    np multiply(np root, long long i, const X &x) {
        assert(L0 <= i && i < R0);
        return multiply_rec(root, L0, R0, i, x);
    }

    template <typename F>
    long long max_right(np root, F check, long long L) {
        assert(L0 <= L && L <= R0 && check(MX::unit()));
        X x = MX::unit();
        return max_right_rec(root, check, L0, R0, L, x);
    }

    template <typename F>
    long long min_left(np root, F check, long long R) {
        assert(L0 <= R && R <= R0 && check(MX::unit()));
        X x = MX::unit();
        return min_left_rec(root, check, L0, R0, R, x);
    }

    void reset() { pid = 0; FREE.clear(); }

    vector<pair<long long, X>> get_all(np root) {
        vector<pair<long long, X>> res;
        auto dfs = [&](auto &dfs, np c) -> void {
            if (!c) return;
            dfs(dfs, c->l);
            res.emplace_back(c->idx, c->x);
            dfs(dfs, c->r);
        };
        dfs(dfs, root);
        return res;
    }

    X get(np root, long long idx) {
        auto dfs = [&](auto &dfs, np c) -> X {
            if (!c) return Monoid::unit();
            if (idx == c->idx) return c->x;
            if (idx < (c->idx)) return dfs(dfs, c->l);
            return dfs(dfs, c->r);
        };
        return dfs(dfs, root);
    }
private:
    void update(np c) {
        c->prod = c->x;
        if (c->l) c->prod = MX::op(c->l->prod, c->prod);
        if (c->r) c->prod = MX::op(c->prod, c->r->prod);
    }

    np copy_node(np c) {
        if (!c || !PERSISTENT) return c;
        pool[pid].idx = c->idx;
        pool[pid].l = c->l;
        pool[pid].r = c->r;
        pool[pid].x = c->x;
        pool[pid].prod = c->prod;
        return &(pool[pid++]);
    }

    np set_rec(np c, long long l, long long r, long long i, X x) {
        if (!c) {
            c = new_node(i, x);
            return c;
        }
        c = copy_node(c);
        if (c->idx == i) {
            c->x = x;
            update(c);
            return c;
        }
        long long m = (l + r) / 2;
        if (i < m) {
            if (c->idx < i) swap(c->idx, i), swap(c->x, x);
            c->l = set_rec(c->l, l, m, i, x);
        }
        if (m <= i) {
            if (i < c->idx) swap(c->idx, i), swap(c->x, x);
            c->r = set_rec(c->r, m, r, i, x);
        }
        update(c);
        return c;
    }

    np multiply_rec(np c, long long l, long long r, long long i, X x) {
        if (!c) {
            c = new_node(i, x);
            return c;
        }
        c = copy_node(c);
        if (c->idx == i) {
            c->x = MX::op(c->x, x);
            update(c);
            return c;
        }
        long long m = (l + r) / 2;
        if (i < m) {
            if (c->idx < i) swap(c->idx, i), swap(c->x, x);
            c->l = multiply_rec(c->l, l, m, i, x);
        }
        if (m <= i) {
            if (i < c->idx) swap(c->idx, i), swap(c->x, x);
            c->r = multiply_rec(c->r, m, r, i, x);
        }
        update(c);
        return c;
    }

    void prod_rec(np c, long long l, long long r, long long ql, long long qr, X &x) {
        chmax(ql, l);
        chmin(qr, r);
        if (ql >= qr || !c) return;
        if (l == ql && r == qr) {
            x = MX::op(x, c->prod);
            return;
        }
        long long m = (l + r) / 2;
        prod_rec(c->l, l, m, ql, qr, x);
        if (ql <= (c->idx) && (c->idx) < qr) x = MX::op(x, c->x);
        prod_rec(c->r, m, r, ql, qr, x);
    }

    template <typename F>
    long long max_right_rec(np c, const F &check, long long l, long long r, long long ql, X &x) {
        if (!c || r <= ql) return R0;
        if (check(MX::op(x, c->prod))) {
            x = MX::op(x, c->prod);
            return R0;
        }
        long long m = (l + r) / 2;
        long long k = max_right_rec(c->l, check, l, m, ql, x);
        if (k != R0) return k;
        if (ql <= (c->idx)) {
            x = MX::op(x, c->x);
            if (!check(x)) return c->idx;
        }
        return max_right_rec(c->r, check, m, r, ql, x);
    }

    template <typename F>
    long long min_left_rec(np c, const F &check, long long l, long long r, long long qr, X &x) {
        if (!c || qr <= l) return L0;
        if (check(MX::op(c->prod, x))) {
            x = MX::op(c->prod, x);
            return L0;
        }
        long long m = (l + r) / 2;
        long long k = min_left_rec(c->r, check, m, r, qr, x);
        if (k != L0) return k;
        if (c->idx < qr) {
            x = MX::op(c->x, x);
            if (!check(x)) return c->idx + 1;
        }
        return min_left_rec(c->l, check, l, m, qr, x);
    }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_SEGTREE_SPARSE