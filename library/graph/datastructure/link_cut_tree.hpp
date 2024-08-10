#ifndef AJAY_LINK_CUT_TREE
#define AJAY_LINK_CUT_TREE

namespace mitsuha{
/*
Have each heavy path in a splay tree with the head on the left and the tail on the right.
Only those that can be directly called by the user are also implemented as int.
Be careful not to forget to push when exploring outside of LCT.
*/
template <typename Node>
struct Link_Cut_Tree {
    using np = Node *;
    int n;
    vector<Node> nodes;

    Link_Cut_Tree(int n = 0) : n(n), nodes(n) { For(i, n) nodes[i] = Node(i); }

    Node *operator[](int v) { return &nodes[v]; }

    // underlying tree root of
    Node *get_root(Node *c) {
        expose(c);
        c->push();
        while (c->l) {
            c = c->l;
            c->push();
        }
        splay(c);
        return c;
    }

    // underlying tree root of
    int get_root(int c) { return get_root(&nodes[c])->idx; }

    // link so that parent(c)==p.
    void link(Node *c, Node *p) {
        evert(c);
        expose(p);
        p->push();
        // no edge -> heavy edge
        assert(!(c->p));
        assert(!(p->r));
        c->p = p;
        p->r = c;
        p->update();
    }

    // link so that parent(c)==p.
    void link(int c, int p) { return link(&nodes[c], &nodes[p]); }

    void cut(Node *a, Node *b) {
        evert(a);
        expose(b);
        assert(!b->p);
        assert((b->l) == a);
        // heavy edge -> no edge
        b->l->p = nullptr;
        b->l = nullptr;
        b->update();
    }

    void cut(int a, int b) { return cut(&nodes[a], &nodes[b]); }

    // Let c be the root of the underlying tree.
    // c is also the root of the splay tree.
    // c is already pushed
    void evert(Node *c) {
        expose(c);
        c->reverse();
        c->push();
    }

    // Let c be the root of the underlying tree.
    // c is also the root of the splay tree.
    void evert(int c) { evert(&nodes[c]); }

    Node *lca(Node *u, Node *v) {
        assert(get_root(u) == get_root(v));
        expose(u);
        return expose(v);
    }

    int lca(int u, int v) { return lca(&nodes[u], &nodes[v])->idx; }

    int dist(int u, int v) {
        evert(u), expose(v);
        return ((*this)[v]->size) - 1;
    }

    Node *jump(Node *u, Node *v, int k) {
        evert(v);
        expose(u);
        assert(0 <= k && k < (u->size));
        while (1) {
            u->push();
            int rs = (u->r ? u->r->size : 0);
            if (k < rs) {
                u = u->r;
                continue;
            }
            if (k == rs) { break; }
            k -= rs + 1;
            u = u->l;
        }
        splay(u);
        return u;
    }

    int jump(int u, int v, int k) {
        auto c = jump((*this)[u], (*this)[v], k);
        return c->idx;
    }


    // Change [root, c] to become one splay tree.
    // c is at the right end and is the root of the splay tree.
    // path query looks at data in c in this state.
    // c is already pushed
    virtual Node *expose(Node *c) {
        Node *now = c;
        Node *rp = nullptr; // Paths created so far
        while (now) {
            splay(now);
            // heavy -> light, light -> heavy.
            if (now->r) { now->add_light(now->r); }
            if (rp) { now->erase_light(rp); }
            now->r = rp;
            now->update();
            rp = now;
            now = now->p;
        }
        splay(c);
        return rp;
    }

    // Change [root, c] to become one splay tree.
    // c is at the right end and is the root of the splay tree.
    // path query looks at data in c in this state.
    int expose(int c) {
        Node *x = expose(&nodes[c]);
        if (!x) return -1;
        return x->idx;
    }

    Node *get_parent(Node *x) {
        expose(x);
        if (!x->l) return nullptr;
        x = x->l;
        while (x->r) x = x->r;
        return x;
    }

    int get_parent(int x) {
        Node *p = get_parent((*this)[x]);
        return (p ? p->idx : -1);
    }

    void set(Node *c, typename Node::VX x) {
        evert(c);
        c->set(x);
    }

    void set(int c, typename Node::VX x) { set((*this)[c], x); }

    typename Node::X prod_path(int a, int b) {
        evert(a), expose(b);
        return (*this)[b]->x;
    }

    // Use node for subtree
    typename Node::X prod_subtree(int v, int root) {
        static_assert(Node::NODE_FOR_SUBTREE);
        if (v == root) {
            evert(root);
            return (*this)[root]->x;
        }
        root = jump(v, root, 1);
        cut(v, root);
        typename Node::X res = (*this)[v]->x;
        link(v, root);
        return res;
    }

    vector<int> collect_heavy_path(int v) {
        np c = (*this)[v];
        while (!is_root(c)) c = c->p;
        vector<int> res;
        auto dfs = [&](auto &dfs, np c, bool rev) -> void {
            if (!rev) {
                if (c->l) dfs(dfs, c->l, rev ^ c->rev);
                res.emplace_back(c->idx);
                if (c->r) dfs(dfs, c->r, rev ^ c->rev);
            } else {
                if (c->r) dfs(dfs, c->r, rev ^ c->rev);
                res.emplace_back(c->idx);
                if (c->l) dfs(dfs, c->l, rev ^ c->rev);
            }
        };
        dfs(dfs, c, false);
        return res;
    }

    void debug_internal() {
        debug("p, l, r, rev");
        auto f = [&](np c) -> int { return (c ? c->idx : -1); };
        For(i, len(nodes)) {
            debug(i, ",", f((*this)[i]->p), f((*this)[i]->l), f((*this)[i]->r),
                  (*this)[i]->rev);
        }
        For(i, len(nodes)) {
            np c = (*this)[i];
            if (c->l) assert(c->l->p == c);
            if (c->r) assert(c->r->p == c);
        }
    }

private:
    // Operations that are completed within the splay tree.
    // In particular, the heavy and light structures do not change.
    // light pointer is taken care of in rotate
    // c is already pushed
    void splay(Node *c) {
        c->push();
        while (!is_root(c)) {
            Node *p = c->p;
            Node *pp = (p ? p->p : nullptr);
            if (state(p) == 0) {
                p->push(), c->push();
                rotate(c);
            }
            elif (state(c) == state(p)) {
                pp->push(), p->push(), c->push();
                rotate(p);
                rotate(c);
            }
            else {
                pp->push(), p->push(), c->push();
                rotate(c);
                rotate(c);
            }
        }
    }


    // Is it the root of the splay tree representing the path?
    // not the underlying tree
    bool is_root(Node *c) { return state(c) == 0; }

    // Operations that are completed within the splay tree. In particular, the heavy and light structures do not change.
    // light edge pointer can be changed
    void rotate(Node *n) {
        // n bring closer to the roots
        Node *pp, *p, *c;
        p = n->p;
        pp = p->p;
        if (p->l == n) {
            c = n->r;
            n->r = p;
            p->l = c;
        } else {
            c = n->l;
            n->l = p;
            p->r = c;
        }
        p->update(), n->update();

        if (pp) {
            if (pp->l == p) pp->l = n;
            elif (pp->r == p) pp->r = n;
            else {
                // light edge pointer changes from (pp-p) to (pp-n)
                pp->change_light(p, n);
            }
        }
        n->p = pp;
        p->p = n;
        if (c) c->p = p;
    }

    inline int state(Node *n) {
        if (!n->p) return 0;
        if (n->p->l == n) return 1;
        if (n->p->r == n) return -1;
        return 0;
    }
};
} // namespace mitsuha
#endif // AJAY_LINK_CUT_TREE