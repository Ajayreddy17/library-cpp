#ifndef AJAY_LCT_NODE_BASE
#define AJAY_LCT_NODE_BASE

namespace mitsuha{
// SUBTREE : If cluster has subtree information
struct LCT_Node_Base {
    using np = LCT_Node_Base*;
    // default
    np l, r, p;
    int idx, size; // size is the number of vertices of heavy path
    bool rev;
    using X = int;
    using VX = int;

    LCT_Node_Base(int i = 0)
            : l(nullptr), r(nullptr), p(nullptr), idx(i), size(1), rev(0) {}

    void update() {
        size = 1;
        if (l) { size += l->size; }
        if (r) { size += r->size; }
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
    void set(VX x) {}

    void add_light(np c) {}
    void erase_light(np c) {}

    // b->x contains subtree value.
    void change_light(np a, np b) {}
};
} // namespace mitsuha
#endif // AJAY_LCT_NODE_BASE