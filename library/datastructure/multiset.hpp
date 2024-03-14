#ifndef AJAY_MULTISET
#define AJAY_MULTISET

#include "library/datastructure/segment_tree/dynamic_segtree_sparse.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
#ifndef LOCAL
Dynamic_SegTree_Sparse<Monoid_Add<long long>, false, (int) 1e6> Meg(-linf, linf);
#else
Dynamic_SegTree_Sparse<Monoid_Add<long long>, false, (int) 1e3> Meg(-linf, linf);
#endif

struct Multiset{
#ifdef LOCAL
    #define ASSERT(c) assert(c)
#else
    #define ASSERT(c) 42
#endif
    using np = decltype(Meg)::np;
    np root;
    long long _size;
    Multiset(): root(Meg.new_root()), _size(0LL){ }

    void insert(long long x, int cnt = 1){
        _size += cnt;
        root = Meg.multiply(root, x, cnt);
    }
    void remove(long long x, int cnt = 1){
        ASSERT(Meg.get(root, x) >= cnt);
        _size -= cnt;
        root = Meg.multiply(root, x, -cnt);
    }
    void erase(long long x, int cnt = 1) { remove(x, cnt); }

    long long size(){ return _size; }

    optional<long long> min_gt(long long x){
        long long ret = Meg.max_right(root, [](long long x){ return not x; }, x + 1);
        return (ret == linf) ? nullopt: make_optional(ret);
    }
    optional<long long> min_geq(long long x){ return min_gt(x - 1); }
    optional<long long> max_lt(long long x){
        long long ret =  Meg.min_left(root, [](long long x){ return not x; }, x);
        return (ret == -linf) ? nullopt: make_optional(ret - 1);
    }
    optional<long long> max_leq(long long x){ return max_lt(x + 1); }

    long long count_gt(long long x){ return Meg.prod(root, x + 1, linf); }
    long long count_geq(long long x){ return Meg.prod(root, x, linf); }
    long long count_lt(long long x){ return Meg.prod(root, -linf, x); }
    long long count_leq(long long x){ return Meg.prod(root, -linf, x + 1); }

    long long count(long long x){ return Meg.get(root, x); }
    // number of x s.t l <= x < r.
    long long count_range(long long l, long long r){ return Meg.prod(root, l, r); }

    long long starting_index(long long x){
        ASSERT(count(x));
        return count_lt(x);
    }
    long long ending_index(long long x){
        ASSERT(count(x));
        return count_leq(x) - 1;
    }
    // index range
    pair<long long, long long> equal_range(long long x){ return make_pair(count_lt(x), count_leq(x)); }

    long long operator[](int idx) {
        ASSERT(idx < Meg.prod_all(root));
        return Meg.max_right(root, [&](long long x){ return x <= idx; }, -linf);
    }
    long long index(int idx) { return (*this)[idx]; }
    long long front(){ return index(0); }
    long long back(){ return index(_size - 1); }
#undef ASSERT
};

std::ostream &operator<<(std::ostream &out, const Multiset &S){
    Multiset M = S;
    out << "{";
    for (int i = 0; i < M.size(); i++){
        out << M[i];
        if (i != M.size() - 1) out << ", ";
    }
    out << "}";
    return out;
}
} // namespace mitsuha
#endif // AJAY_MULTISET