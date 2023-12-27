#ifndef AJAY_SPLAY_TREE_COM_MONOID
#define AJAY_SPLAY_TREE_COM_MONOID

#include "library/datastructure/splay_tree/splay_tree.hpp"

namespace mitsuha{
namespace SplayTreeNodes {
    template <typename Monoid>
    struct Node_CM {
        using Monoid_X = Monoid;
        using X = typename Monoid::value_type;
        using value_type = X;
        using operator_type = int;
        using np = Node_CM *;

        np p, l, r;
        X x, prod;
        unsigned int size;
        bool rev;

        static void new_node(np n, const X &x) {
            n->p = n->l = n->r = nullptr;
            n->x = n->prod = x;
            n->size = 1;
            n->rev = 0;
        }

        void update() {
            size = 1;
            prod = x;
            if (l) {
                size += l->size;
                prod = Monoid::op(l->prod, prod);
            }
            if (r) {
                size += r->size;
                prod = Monoid::op(prod, r->prod);
            }
        }

        void prop() {
            if (rev) {
                if (l) {
                    l->rev ^= 1;
                    swap(l->l, l->r);
                }
                if (r) {
                    r->rev ^= 1;
                    swap(r->l, r->r);
                }
                rev = 0;
            }
        }

        // Anything called other than update or prop is expected to be called after splay.
        // Therefore, it can be assumed that update and prop have been completed at that point.
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
            swap(l, r);
            rev ^= 1;
        }
    };
    template <typename Monoid, int NODES>
    using SplayTree_Commutative_Monoid = SplayTree<Node_CM<Monoid>, NODES>;
} // namespace SplayTreeNodes

using SplayTreeNodes::SplayTree_Commutative_Monoid;
} // namespace mitsuha
#endif // AJAY_SPLAY_TREE_COM_MONOID