#ifndef AJAY_DYNAMIC_UNIONFIND
#define AJAY_DYNAMIC_UNIONFIND

#include "library/datastructure/dynamic_array.hpp"

namespace mitsuha{
template <bool PERSISTENT>
struct Dynamic_UnionFind {
    // Without path compression
    Dynamic_Array<int, PERSISTENT> PA;
    using np = typename decltype(PA)::np;

    Dynamic_UnionFind(int N) : PA(15 * N, -1) {}

    np new_root() { return PA.new_root(); }

    int root(np c, int x) {
        while (1) {
            int p = PA.get(c, x);
            assert(x != p);
            if (p < 0) break;
            x = p;
        }
        return x;
    }

    pair<np, bool> merge(np c, int x, int y) {
        x = root(c, x), y = root(c, y);
        if (x == y) return {c, false};
        if (-PA.get(c, x) < -PA.get(c, y)) swap(x, y);
        int new_sz = PA.get(c, x) + PA.get(c, y);
        c = PA.set(c, x, new_sz);
        assert(PA.get(c, x) == new_sz);
        c = PA.set(c, y, x);
        assert(PA.get(c, y) == x);
        return {c, true};
    }

    long long size(np c, int x) { return -PA.get(c, root(c, x)); }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_UNIONFIND