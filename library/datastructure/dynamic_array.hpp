#ifndef AJAY_DYNAMIC_ARRAY
#define AJAY_DYNAMIC_ARRAY

namespace mitsuha{
template <typename T, bool PERSISTENT>
struct Dynamic_Array {
    static constexpr int LOG = 4;
    static constexpr int MASK = (1 << LOG) - 1;
    struct Node {
        T x;
        Node* ch[1 << LOG] = {};
    };
    const int NODES;
    Node* pool;
    int pid;
    using np = Node*;
    const T x0;

    Dynamic_Array(int NODES, T default_value) : NODES(NODES), pid(0), x0(default_value) {
        pool = new Node[NODES];
    }
    ~Dynamic_Array() { delete[] pool; }
    np new_root() {
        pool[pid].x = x0;
        fill(pool[pid].ch, pool[pid].ch + (1 << LOG), nullptr);
        return &(pool[pid++]);
    }

    np new_node(vector<T> dat) {
        np root = new_root();
        For(i, len(dat)) root = set(root, i, dat[i], false);
        return root;
    }

    T get(np c, int idx) {
        if (!c) return x0;
        if (idx == 0) return c->x;
        return get(c->ch[idx & MASK], (idx - 1) >> LOG);
    }

    np set(np c, int idx, T x, bool make_copy = true) {
        c = (c ? copy_node(c, make_copy) : new_root());
        if (idx == 0) {
            c->x = x;
            return c;
        }
        c->ch[idx & MASK] = set(c->ch[idx & MASK], (idx - 1) >> LOG, x);
        return c;
    }
    
private:
    np copy_node(np c, bool make_copy) {
        if (!make_copy || !PERSISTENT) return c;
        pool[pid].x = c->x;
        For(k, (1 << LOG)) pool[pid].ch[k] = c->ch[k];
        return &(pool[pid++]);
    }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_ARRAY