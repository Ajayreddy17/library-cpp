#ifndef AJAY_SPLAY_TREE_BASIC
#define AJAY_SPLAY_TREE_BASIC

#include "library/datastructure/splay_tree/splay_tree.hpp"

namespace mitsuha{
namespace SplayTreeNodes {
    template <typename S>
    struct Node_Basic {
        using value_type = S;
        using operator_type = int;
        using np = Node_Basic *;

        np p, l, r;
        bool rev;
        S x;
        unsigned int size;

        static void new_node(np n, const S &x) {
            n->p = n->l = n->r = nullptr;
            n->x = x, n->size = 1, n->rev = 0;
        }

        void update() {
            size = 1;
            if (l) { size += l->size; }
            if (r) { size += r->size; }
        }

        void prop() {
            if (rev) {
                if (l) { l->rev ^= 1, swap(l->l, l->r); }
                if (r) { r->rev ^= 1, swap(r->l, r->r); }
                rev = 0;
            }
        }

        // Anything called other than update or prop is expected to be called after splay.
        // Therefore, it can be assumed that update and prop have been completed at that point.
        S get() { return x; }
        void set(const S &xx) {
            x = xx;
            update();
        }
        void reverse() {
            swap(l, r);
            rev ^= 1;
        }
    };
    template <typename S, int NODES>
    using SplayTree_Basic = SplayTree<Node_Basic<S>, NODES>;
} // namespace SplayTreeNodes
using SplayTreeNodes::SplayTree_Basic;
} // namespace mitsuha
#endif // AJAY_SPLAY_TREE_BASIC