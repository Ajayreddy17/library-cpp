#ifndef AJAY_SPLAY_TREE_MONOID
#define AJAY_SPLAY_TREE_MONOID

#include "library/datastructure/splay_tree/splay_tree.hpp"

namespace mitsuha {
namespace SplayTreeNodes {
template <typename Monoid>
struct Node_Monoid {
    using Monoid_X = Monoid;
    using X = typename Monoid::value_type;
    using value_type = X;
    using operator_type = int; // Just define it for now.
    using np = Node_Monoid *;

    np p, l, r;
    X x, prod, rev_prod;
    unsigned int size;
    bool rev;

    static void new_node(np n, const X &x) {
        n->p = n->l = n->r = nullptr;
        n->x = n->prod = n->rev_prod = x;
        n->size = 1;
        n->rev = 0;
    }

    void update() {
        size = 1;
        prod = rev_prod = x;
        if (l) {
            size += l->size;
            prod = Monoid::op(l->prod, prod);
            rev_prod = Monoid::op(rev_prod, l->rev_prod);
        }
        if (r) {
            size += r->size;
            prod = Monoid::op(prod, r->prod);
            rev_prod = Monoid::op(r->rev_prod, rev_prod);
        }
    }

    void prop() {
        if (rev) {
            if (l) {
                l->rev ^= 1;
                swap(l->l, l->r);
                swap(l->prod, l->rev_prod);
            }
            if (r) {
                r->rev ^= 1;
                swap(r->l, r->r);
                swap(r->prod, r->rev_prod);
            }
            rev = 0;
        }
    }

    // Anything called other than update or prop is assumed to be after the splay operation. 
    // Therefore, it can be assumed that update and prop have already been applied at that point
    X get() { return x; }
    void set(const X &xx) {
        x = xx;
        update();
    }
    void multiply(const X &xx) {
        x = Monoid::op(x, xx);
        update();
    }
    void reverse() {
        swap(prod, rev_prod);
        swap(l, r);
        rev ^= 1;
    }
};
template <typename Monoid>
using SplayTree_Monoid = SplayTree<Node_Monoid<Monoid>>;
} // namespace SplayTreeNodes

using SplayTreeNodes::SplayTree_Monoid;
} // namespace mitsuha
#endif // AJAY_SPLAY_TREE_MONOID