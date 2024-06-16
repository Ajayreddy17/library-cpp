#ifndef AJAY_DYNAMIC_LAZY_SEGTREE
#define AJAY_DYNAMIC_LAZY_SEGTREE

namespace mitsuha{
template <typename ActedMonoid, bool PERSISTENT = false, int NODES = 1 << 22>
struct Dynamic_Lazy_SegTree {
    using AM = ActedMonoid;
    using MX = typename AM::Monoid_X;
    using MA = typename AM::Monoid_A;
    using X = typename AM::X;
    using A = typename AM::A;
    using F = function<X(long long, long long)>;
    F default_prod;

    struct Node {
        Node *l, *r;
        X x;
        A lazy;
    };

    const long long L0, R0;
    Node *pool;
    int pid;
    using np = Node *;

    Dynamic_Lazy_SegTree(
            long long L0, long long R0, F default_prod = [](long long l, long long r) -> X { return MX::unit(); })
            : default_prod(default_prod), L0(L0), R0(R0), pid(0) {
        pool = new Node[NODES];
    }

    np new_root() { return new_node(L0, R0); }

    np new_node(const X x) {
        pool[pid].l = pool[pid].r = nullptr;
        pool[pid].x = x;
        pool[pid].lazy = MA::unit();
        return &(pool[pid++]);
    }

    np new_node(long long l, long long r) { return new_node(default_prod(l, r)); }
    np new_node() { return new_node(L0, R0); }

    np new_node(const vector<X> &dat) {
        assert(L0 == 0 && R0 == len(dat));
        auto dfs = [&](auto &dfs, long long l, long long r) -> Node * {
            if (l == r) return nullptr;
            if (r == l + 1) return new_node(dat[l]);
            long long m = (l + r) / 2;
            np l_root = dfs(dfs, l, m), r_root = dfs(dfs, m, r);
            X x = MX::op(l_root->x, r_root->x);
            np root = new_node(x);
            root->l = l_root, root->r = r_root;
            return root;
        };
        return dfs(dfs, 0, len(dat));
    }

    X prod(np root, long long l, long long r) {
        if (l == r || !root) return MX::unit();
        assert(pid && L0 <= l && l < r && r <= R0);
        X x = MX::unit();
        prod_rec(root, L0, R0, l, r, x, MA::unit());
        return x;
    }

    X prod_all(np root) { return prod(root, L0, R0); }

    np set(np root, long long i, const X &x) {
        assert(pid && L0 <= i && i < R0);
        return set_rec(root, L0, R0, i, x);
    }

    np multiply(np root, long long i, const X &x) {
        assert(pid && L0 <= i && i < R0);
        return multiply_rec(root, L0, R0, i, x);
    }

    np apply(np root, long long l, long long r, const A &a) {
        if (l == r) return root;
        assert(pid && L0 <= l && l < r && r <= R0);
        return apply_rec(root, L0, R0, l, r, a);
    }

    template <typename F>
    long long max_right(np root, F check, long long L) {
        assert(pid && L0 <= L && L <= R0 && check(MX::unit()));
        X x = MX::unit();
        return max_right_rec(root, check, L0, R0, L, x);
    }

    template <typename F>
    long long min_left(np root, F check, long long R) {
        assert(pid && L0 <= R && R <= R0 && check(MX::unit()));
        X x = MX::unit();
        return min_left_rec(root, check, L0, R0, R, x);
    }

    // f(idx, val)
    template <typename F>
    void enumerate(np root, F f) {
        auto dfs = [&](auto &dfs, np c, long long l, long long r, A a) -> void {
            if (!c) return;
            if (r - l == 1) {
                f(l, AM::act(c->x, a, 1));
                return;
            }
            long long m = (l + r) / 2;
            a = MA::op(c->lazy, a);
            dfs(dfs, c->l, l, m, a);
            dfs(dfs, c->r, m, r, a);
        };
        dfs(dfs, root, L0, R0, MA::unit());
    }

    void reset() { pid = 0; }

private:
    np copy_node(np c) {
        if (!c || !PERSISTENT) return c;
        pool[pid].l = c->l, pool[pid].r = c->r;
        pool[pid].x = c->x;
        pool[pid].lazy = c->lazy;
        return &(pool[pid++]);
    }

    void prop(np c, long long l, long long r) {
        assert(r - l >= 2);
        long long m = (l + r) / 2;
        if (c->lazy == MA::unit()) return;
        c->l = (c->l ? copy_node(c->l) : new_node(l, m));
        c->l->x = AM::act(c->l->x, c->lazy, m - l);
        c->l->lazy = MA::op(c->l->lazy, c->lazy);
        c->r = (c->r ? copy_node(c->r) : new_node(m, r));
        c->r->x = AM::act(c->r->x, c->lazy, r - m);
        c->r->lazy = MA::op(c->r->lazy, c->lazy);
        c->lazy = MA::unit();
    }

    np set_rec(np c, long long l, long long r, long long i, const X &x) {
        if (r == l + 1) {
            c = copy_node(c);
            c->x = x;
            c->lazy = MA::unit();
            return c;
        }
        prop(c, l, r);
        long long m = (l + r) / 2;
        if (!c->l) c->l = new_node(l, m);
        if (!c->r) c->r = new_node(m, r);

        c = copy_node(c);
        if (i < m) {
            c->l = set_rec(c->l, l, m, i, x);
        } else {
            c->r = set_rec(c->r, m, r, i, x);
        }
        c->x = MX::op(c->l->x, c->r->x);
        return c;
    }

    np multiply_rec(np c, long long l, long long r, long long i, const X &x) {
        if (r == l + 1) {
            c = copy_node(c);
            c->x = MX::op(c->x, x);
            c->lazy = MA::unit();
            return c;
        }
        prop(c, l, r);
        long long m = (l + r) / 2;
        if (!c->l) c->l = new_node(l, m);
        if (!c->r) c->r = new_node(m, r);

        c = copy_node(c);
        if (i < m) {
            c->l = multiply_rec(c->l, l, m, i, x);
        } else {
            c->r = multiply_rec(c->r, m, r, i, x);
        }
        c->x = MX::op(c->l->x, c->r->x);
        return c;
    }

    void prod_rec(np c, long long l, long long r, long long ql, long long qr, X &x, A lazy) {
        ql = max(ql, l);
        qr = min(qr, r);
        if (ql >= qr) return;
        if (!c) {
            x = MX::op(x, AM::act(default_prod(ql, qr), lazy, qr - ql));
            return;
        }
        if (l == ql && r == qr) {
            x = MX::op(x, AM::act(c->x, lazy, r - l));
            return;
        }
        long long m = (l + r) / 2;
        lazy = MA::op(c->lazy, lazy);
        prod_rec(c->l, l, m, ql, qr, x, lazy);
        prod_rec(c->r, m, r, ql, qr, x, lazy);
    }

    np apply_rec(np c, long long l, long long r, long long ql, long long qr, const A &a) {
        if (!c) c = new_node(l, r);
        ql = max(ql, l);
        qr = min(qr, r);
        if (ql >= qr) return c;
        if (l == ql && r == qr) {
            c = copy_node(c);
            c->x = AM::act(c->x, a, r - l);
            c->lazy = MA::op(c->lazy, a);
            return c;
        }
        prop(c, l, r);
        long long m = (l + r) / 2;
        c = copy_node(c);
        c->l = apply_rec(c->l, l, m, ql, qr, a);
        c->r = apply_rec(c->r, m, r, ql, qr, a);
        c->x = MX::op(c->l->x, c->r->x);
        return c;
    }

    template <typename F>
    long long max_right_rec(np c, const F &check, long long l, long long r, long long ql, X &x) {
        if (r <= ql) return r;
        if (!c) c = new_node(l, r);
        ql = max(ql, l);
        if (l == ql && check(MX::op(x, c->x))) {
            x = MX::op(x, c->x);
            return r;
        }
        if (r == l + 1) return l;
        prop(c, l, r);
        long long m = (l + r) / 2;
        long long k = max_right_rec(c->l, check, l, m, ql, x);
        if (k < m) return k;
        return max_right_rec(c->r, check, m, r, ql, x);
    }

    template <typename F>
    long long min_left_rec(np c, const F &check, long long l, long long r, long long qr, X &x) {
        if (qr <= l) return l;
        if (!c) c = new_node(l, r);
        qr = min(qr, r);
        if (r == qr && check(MX::op(c->x, x))) {
            x = MX::op(c->x, x);
            return l;
        }
        if (r == l + 1) return r;
        prop(c, l, r);
        long long m = (l + r) / 2;
        long long k = min_left_rec(c->r, check, m, r, qr, x);
        if (m < k) return k;
        return min_left_rec(c->l, check, l, m, qr, x);
    }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_LAZY_SEGTREE