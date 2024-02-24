#ifndef AJAY_ROLLBACK_UNIONFIND
#define AJAY_ROLLBACK_UNIONFIND

#include "library/datastructure/rollback_array.hpp"

namespace mitsuha{
struct Rollback_UnionFind {
    Rollback_Array<int> dat; // parent or size
    Rollback_UnionFind(int n) : dat(vector<int>(n, -1)) {}

    int operator[](int v) {
        while (dat.get(v) >= 0) v = dat.get(v);
        return v;
    }
    long long size(int v) { return -dat.get((*this)[v]); }
    int time() { return dat.time(); }
    void rollback(int t) { dat.rollback(t); }
    void reset() { rollback(0); }

    bool merge(int a, int b) {
        a = (*this)[a], b = (*this)[b];
        if (a == b) return false;
        if (dat.get(a) > dat.get(b)) swap(a, b);
        dat.set(a, dat.get(a) + dat.get(b));
        dat.set(b, a);
        return true;
    }
};
} // namespace mitsuha
#endif // AJAY_ROLLBACK_UNIONFIND