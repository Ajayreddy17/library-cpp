#ifndef AJAY_LCT_NODE_COM_MONOID
#define AJAY_LCT_NODE_COM_MONOID

namespace mitsuha{
// SUBTREE : If cluster has subtree information
template <typename Monoid, bool SUBTREE = false>
struct LCT_Node_Commutative_Monoid {
    static_assert(Monoid::commute);
    static constexpr bool NODE_FOR_SUBTREE = SUBTREE;
    using np = LCT_Node_Commutative_Monoid *;
    // default
    np l, r, p;
    int idx, size; // size is the number of vertices of heavy path
    bool rev;
    // Define each purpose.
    using MX = Monoid;
    using X = typename MX::value_type;
    using VX = X;
    X x, vx, mid;

    LCT_Node_Commutative_Monoid(int i = 0)
            : l(nullptr),
              r(nullptr),
              p(nullptr),
              idx(i),
              size(1),
              rev(0),
              x(MX::unit()),
              vx(MX::unit()),
              mid(MX::unit()) {}

    void update() {
        size = 1;
        x = vx;
        if constexpr (SUBTREE) x = MX::op(x, mid);
        if (l) { size += l->size, x = Monoid::op(l->x, x); }
        if (r) { size += r->size, x = Monoid::op(x, r->x); }
    }

    void push() {
        if (rev) {
            if (l) l->reverse();
            if (r) r->reverse();
            rev = 0;
        }
    }

    // Also performs reverse of data
    void reverse() {
        rev ^= 1;
        swap(l, r);
    }

    // Expose and update are performed within the LCT, so just make changes here.
    void set(VX x) { vx = x; }

    void add_light(np c) {
        if constexpr (SUBTREE) mid = MX::op(mid, c->x);
    }
    void erase_light(np c) {
        if constexpr (SUBTREE) mid = MX::op(mid, MX::inverse(c->x));
    }
    
    // b->x contains subtree value.
    void change_light(np a, np b) {}
};
} // namespace mitsuha
#endif // AJAY_LCT_NODE_COM_MONOID