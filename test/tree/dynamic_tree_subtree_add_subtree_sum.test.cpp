#define PROBLEM \
    "https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum"
#include <library/template.hpp>

#include <library/graph/datastructure/link_cut_tree.hpp>

/*
sm has lazy already applied.
However, lazy_light - cancel from light edges is not yet applied.
*/
struct Node {
    using np = Node *;
    // Default
    np l, r, p;
    int idx, size; // size is the number of vertices in the heavy path
    bool rev;
    struct X {
        long long cnt, sum;
    };

    using VX = X;
    // Define per purpose.
    X x, vx, mid;
    long long lazy, lazy_light, cancel;

    Node(int i = 0)
            : l(nullptr),
                r(nullptr),
                p(nullptr),
                idx(i),
                size(1),
                rev(0),
                x({1, 0}),
                vx({1, 0}),
                mid({0, 0}),
                lazy(0),
                lazy_light(0),
                cancel(0) {}

    void update() {
        size = 1;
        x.cnt = vx.cnt + mid.cnt, x.sum = vx.sum + mid.sum;
        if (l) { size += l->size, x.sum += l->x.sum, x.cnt += l->x.cnt; }
        if (r) { size += r->size, x.sum += r->x.sum, x.cnt += r->x.cnt; }
    }

    void push() {
        if (rev) {
            if (l) l->reverse();
            if (r) r->reverse();
        }
        if (lazy) {
            if (l) l->apply(lazy);
            if (r) r->apply(lazy);
        }
        rev = 0;
        lazy = 0;
    }

    void apply(long long a) {
        vx.sum += a;
        x.sum += a * (x.cnt);
        lazy += a;
        lazy_light += a;
        mid.sum += (mid.cnt * a);
    }

    // Also reverse the data
    void reverse() {
        rev ^= 1;
        swap(l, r);
    }

    // expose and update are done inside LCT, so only modify here
    void set(X x) { vx = x; }

    void add_light(np c) {
        mid.cnt += c->x.cnt, mid.sum += c->x.sum;
        assert(c->cancel == 0);
        c->cancel = lazy_light;
    }
    void erase_light(np c) {
        c->apply(lazy_light - (c->cancel));
        c->cancel = 0;
        mid.cnt -= c->x.cnt, mid.sum -= c->x.sum;
    }

    // b->x contains the subtree value.
    void change_light(np a, np b) {
        b->cancel = a->cancel;
        a->cancel = 0;
    }
};

void solve() {
    LL(N, Q);
    Link_Cut_Tree<Node> LCT(N);
    VT(long long, A, N);
    For(i, N) { LCT.set(i, {1, A[i]}); }
    For(N - 1) {
        INT(a, b);
        LCT.link(a, b);
    }

    For(q, Q) {
        LL(t);
        if (t == 0) {
            LL(a, b, c, d);
            LCT.cut(a, b);
            LCT.link(c, d);
        }
        if (t == 1) {
            LL(v, p, x);
            LCT.cut(v, p);
            LCT[v]->apply(x);
            LCT[v]->push();
            LCT.link(v, p);
        }
        if (t == 2) {
            LL(c, p);
            LCT.cut(c, p);
            print(LCT[c]->x.sum);
            LCT.link(c, p);
        }
    }
}

signed main() {
    solve();
    return 0;
}
