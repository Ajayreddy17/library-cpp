#ifndef AJAY_LCT_NODE_MONOID
#define AJAY_LCT_NODE_MONOID


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/lct_node_monoid.hpp

template <typename Monoid>
struct LCT_Node_Monoid {
    using np = LCT_Node_Monoid *;
    // default
    np l, r, p;
    int idx, size; // size is the number of vertices of the heavy path
    bool rev;
    // define per purpose.
    using MX = Monoid;
    using X = typename MX::value_type;
    using VX = X;

    X x, rx, vx;

    LCT_Node_Monoid(int i = 0)
            : l(nullptr),
                r(nullptr),
                p(nullptr),
                idx(i),
                size(1),
                rev(0),
                x(MX::unit()),
                rx(MX::unit()),
                vx(MX::unit()) {}

    void update() {
        size = 1;
        x = vx, rx = vx;
        if (l) {
            size += l->size, x = Monoid::op(l->x, x), rx = Monoid::op(rx, l->rx);
        }
        if (r) {
            size += r->size, x = Monoid::op(x, r->x), rx = Monoid::op(r->rx, rx);
        }
    }

    void push() {
        if (rev) {
            if (l) l->reverse();
            if (r) r->reverse();
            rev = 0;
        }
    }

    // also reverse the data
    void reverse() {
        rev ^= 1;
        swap(l, r);
        swap(x, rx);
    }

    // expose and update are done inside LCT, so only modify here
    void set(VX x) { vx = x; }

    void add_light(np c) {}
    void erase_light(np c) {}
    void change_light(np a, np b) {}
};
} // namespace mitsuha
#endif // AJAY_LCT_NODE_MONOID
