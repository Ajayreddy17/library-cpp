#ifndef AJAY_RBST_MONOID
#define AJAY_RBST_MONOID

namespace mitsuha{
template <typename Monoid, bool PERSISTENT, int NODES>
struct RBST_Monoid {
    using X = typename Monoid::value_type;

    struct Node {
        Node *l, *r;
        X x, prod, rev_prod; // rev 反映済
        unsigned int size;
        bool rev;
    };

    Node *pool;
    int pid;
    using np = Node *;

    RBST_Monoid() : pid(0) { pool = new Node[NODES]; }

    void reset() { pid = 0; }

    np new_node(const X &x) {
        pool[pid].l = pool[pid].r = nullptr;
        pool[pid].x = x;
        pool[pid].prod = x;
        pool[pid].rev_prod = x;
        pool[pid].size = 1;
        pool[pid].rev = 0;
        return &(pool[pid++]);
    }

    np new_node(const vector<X> &dat) {
        auto dfs = [&](auto &dfs, unsigned int l, unsigned int r) -> np {
            if (l == r) return nullptr;
            if (r == l + 1) return new_node(dat[l]);
            unsigned int m = (l + r) / 2;
            np l_root = dfs(dfs, l, m);
            np r_root = dfs(dfs, m + 1, r);
            np root = new_node(dat[m]);
            root->l = l_root, root->r = r_root;
            update(root);
            return root;
        };
        return dfs(dfs, 0, len(dat));
    }

    np copy_node(np &n) {
        if (!n || !PERSISTENT) return n;
        pool[pid].l = n->l, pool[pid].r = n->r;
        pool[pid].x = n->x;
        pool[pid].prod = n->prod;
        pool[pid].rev_prod = n->rev_prod;
        pool[pid].size = n->size;
        pool[pid].rev = n->rev;
        return &(pool[pid++]);
    }

    np merge(np l_root, np r_root) { return merge_rec(l_root, r_root); }
    np merge3(np a, np b, np c) { return merge(merge(a, b), c); }
    np merge4(np a, np b, np c, np d) { return merge(merge(merge(a, b), c), d); }
    pair<np, np> split(np root, unsigned int k) {
        if (!root) {
            assert(k == 0);
            return {nullptr, nullptr};
        }
        assert(0 <= k && k <= root->size);
        return split_rec(root, k);
    }
    tuple<np, np, np> split3(np root, unsigned int l, unsigned int r) {
        np nm, nr;
        tie(root, nr) = split(root, r);
        tie(root, nm) = split(root, l);
        return {root, nm, nr};
    }
    tuple<np, np, np, np> split4(np root, unsigned int i, unsigned int j, unsigned int k) {
        np d;
        tie(root, d) = split(root, k);
        auto [a, b, c] = split3(root, i, j);
        return {a, b, c, d};
    }

    X prod(np root, unsigned int l, unsigned int r) {
        if (l == r) return Monoid::unit();
        return prod_rec(root, l, r, false);
    }
    X prod(np root) { return (root ? root->prod : Monoid::unit()); }

    np reverse(np root, unsigned int l, unsigned int r) {
        assert(0 <= l && l <= r && r <= root->size);
        if (r - l <= 1) return root;
        auto [nl, nm, nr] = split3(root, l, r);
        nm->rev ^= 1;
        swap(nm->l, nm->r);
        swap(nm->prod, nm->rev_prod);
        return merge3(nl, nm, nr);
    }

    np set(np root, unsigned int k, const X &x) { return set_rec(root, k, x); }
    np multiply(np root, unsigned int k, const X &x) { return multiply_rec(root, k, x); }
    X get(np root, unsigned int k) { return get_rec(root, k, false); }

    vector<X> get_all(np root) {
        vector<X> res;
        auto dfs = [&](auto &dfs, np root, bool rev) -> void {
            if (!root) return;
            dfs(dfs, (rev ? root->r : root->l), rev ^ root->rev);
            res.eb(root->x);
            dfs(dfs, (rev ? root->l : root->r), rev ^ root->rev);
        };
        dfs(dfs, root, 0);
        return res;
    }

    template <typename F>
    pair<np, np> split_max_right(np root, const F check) {
        assert(check(Monoid::unit()));
        X x = Monoid::unit();
        return split_max_right_rec(root, check, x);
    }

private:
    inline unsigned int xor128() {
        static unsigned int x = 123456789;
        static unsigned int y = 362436069;
        static unsigned int z = 521288629;
        static unsigned int w = 88675123;
        unsigned int t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }

    void prop(np c) {
        if (c->rev) {
            if (c->l) {
                c->l = copy_node(c->l);
                c->l->rev ^= 1;
                swap(c->l->l, c->l->r);
                swap(c->l->prod, c->l->rev_prod);
            }
            if (c->r) {
                c->r = copy_node(c->r);
                c->r->rev ^= 1;
                swap(c->r->l, c->r->r);
                swap(c->r->prod, c->r->rev_prod);
            }
            c->rev = 0;
        }
    }

    void update(np c) {
        c->size = 1;
        c->prod = c->rev_prod = c->x;
        if (c->l) {
            c->size += c->l->size;
            c->prod = Monoid::op(c->l->prod, c->prod);
            c->rev_prod = Monoid::op(c->rev_prod, c->l->rev_prod);
        }
        if (c->r) {
            c->size += c->r->size;
            c->prod = Monoid::op(c->prod, c->r->prod);
            c->rev_prod = Monoid::op(c->r->rev_prod, c->rev_prod);
        }
    }

    np merge_rec(np l_root, np r_root) {
        if (!l_root) return r_root;
        if (!r_root) return l_root;
        unsigned int sl = l_root->size, sr = r_root->size;
        if (xor128() % (sl + sr) < sl) {
            prop(l_root);
            l_root = copy_node(l_root);
            l_root->r = merge_rec(l_root->r, r_root);
            update(l_root);
            return l_root;
        }
        prop(r_root);
        r_root = copy_node(r_root);
        r_root->l = merge_rec(l_root, r_root->l);
        update(r_root);
        return r_root;
    }

    pair<np, np> split_rec(np root, unsigned int k) {
        if (!root) return {nullptr, nullptr};
        prop(root);
        unsigned int sl = (root->l ? root->l->size : 0);
        if (k <= sl) {
            auto [nl, nr] = split_rec(root->l, k);
            root = copy_node(root);
            root->l = nr;
            update(root);
            return {nl, root};
        }
        auto [nl, nr] = split_rec(root->r, k - (1 + sl));
        root = copy_node(root);
        root->r = nl;
        update(root);
        return {root, nr};
    }

    np set_rec(np root, unsigned int k, const X &x) {
        if (!root) return root;
        prop(root);
        unsigned int sl = (root->l ? root->l->size : 0);
        if (k < sl) {
            root = copy_node(root);
            root->l = set_rec(root->l, k, x);
            update(root);
            return root;
        }
        if (k == sl) {
            root = copy_node(root);
            root->x = x;
            update(root);
            return root;
        }
        root = copy_node(root);
        root->r = set_rec(root->r, k - (1 + sl), x);
        update(root);
        return root;
    }

    np multiply_rec(np root, unsigned int k, const X &x) {
        if (!root) return root;
        prop(root);
        unsigned int sl = (root->l ? root->l->size : 0);
        if (k < sl) {
            root = copy_node(root);
            root->l = multiply_rec(root->l, k, x);
            update(root);
            return root;
        }
        if (k == sl) {
            root = copy_node(root);
            root->x = Monoid::op(root->x, x);
            update(root);
            return root;
        }
        root = copy_node(root);
        root->r = multiply_rec(root->r, k - (1 + sl), x);
        update(root);
        return root;
    }

    X prod_rec(np root, unsigned int l, unsigned int r, bool rev) {
        if (l == 0 && r == root->size) {
            return (rev ? root->rev_prod : root->prod);
        }
        np left = (rev ? root->r : root->l);
        np right = (rev ? root->l : root->r);
        unsigned int sl = (left ? left->size : 0);
        X res = Monoid::unit();
        if (l < sl) {
            X y = prod_rec(left, l, min(r, sl), rev ^ root->rev);
            res = Monoid::op(res, y);
        }
        if (l <= sl && sl < r) res = Monoid::op(res, root->x);
        unsigned int k = 1 + sl;
        if (k < r) {
            X y = prod_rec(right, max(k, l) - k, r - k, rev ^ root->rev);
            res = Monoid::op(res, y);
        }
        return res;
    }

    X get_rec(np root, unsigned int k, bool rev) {
        np left = (rev ? root->r : root->l);
        np right = (rev ? root->l : root->r);
        unsigned int sl = (left ? left->size : 0);
        if (k == sl) return root->x;
        rev ^= root->rev;
        if (k < sl) return get_rec(left, k, rev);
        return get_rec(right, k - (1 + sl), rev);
    }

    template <typename F>
    pair<np, np> split_max_right_rec(np root, const F &check, X &x) {
        if (!root) return {nullptr, nullptr};
        prop(root);
        root = copy_node(root);
        X y = Monoid::op(x, root->prod);
        if (check(y)) {
            x = y;
            return {root, nullptr};
        }
        np left = root->l, right = root->r;
        if (left) {
            X y = Monoid::op(x, root->l->prod);
            if (!check(y)) {
                auto [n1, n2] = split_max_right_rec(left, check, x);
                root->l = n2;
                update(root);
                return {n1, root};
            }
            x = y;
        }
        y = Monoid::op(x, root->x);
        if (!check(y)) {
            root->l = nullptr;
            update(root);
            return {left, root};
        }
        x = y;
        auto [n1, n2] = split_max_right_rec(right, check, x);
        root->r = n1;
        update(root);
        return {root, n2};
    }
};
} // namespace mitsuha
#endif // AJAY_RBST_MONOID