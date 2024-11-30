#ifndef AJAY_MULTISET
#define AJAY_MULTISET

#include "library/datastructure/segment_tree/dynamic_segtree_sparse.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
namespace internal{
#ifndef LOCAL
Dynamic_SegTree_Sparse<Monoid_Add<long long>, false> Seg(1'000'000, -linf, linf);
#else
Dynamic_SegTree_Sparse<Monoid_Add<long long>, false> Seg(1'000, -linf, linf);
#endif
}

struct Multiset{
    using np = decltype(internal::Seg)::np;
    np root;
    long long _size;
    Multiset(): root(internal::Seg.new_root()), _size(0LL){ }

    void insert(long long x, int cnt = 1){
        _size += cnt;
        root = internal::Seg.multiply(root, x, cnt);
    }
    void add(long long x, int cnt = 1){ insert(x, cnt); }
    void remove(long long x, int cnt = 1){
        Assert(internal::Seg.get(root, x) >= cnt);
        _size -= cnt;
        root = internal::Seg.multiply(root, x, -cnt);
    }
    void erase(long long x, int cnt = 1) { remove(x, cnt); }

    long long size(){ return _size; }

    optional<long long> min_gt(long long x){
        long long ret = internal::Seg.max_right(root, x + 1, [](long long x){ return not x; });
        return (ret == linf) ? nullopt: make_optional(ret);
    }
    optional<long long> min_geq(long long x){ return min_gt(x - 1); }
    optional<long long> max_lt(long long x){
        long long ret =  internal::Seg.min_left(root, x, [](long long x){ return not x; });
        return (ret == -linf) ? nullopt: make_optional(ret - 1);
    }
    optional<long long> max_leq(long long x){ return max_lt(x + 1); }

    long long count_gt(long long x) { return internal::Seg.prod(root, x + 1, linf); }
    long long count_geq(long long x){ return internal::Seg.prod(root, x, linf); }
    long long count_lt(long long x) { return internal::Seg.prod(root, -linf, x); }
    long long count_leq(long long x){ return internal::Seg.prod(root, -linf, x + 1); }

    long long count(long long x){ return internal::Seg.get(root, x); }
    // number of x s.t l <= x < r.
    long long count_range(long long l, long long r){ return internal::Seg.prod(root, l, r); }

    long long starting_index(long long x){
        Assert(count(x));
        return count_lt(x);
    }
    long long ending_index(long long x){
        Assert(count(x));
        return count_leq(x) - 1;
    }

    long long operator[](int idx) {
        Assert(idx < internal::Seg.prod_all(root));
        return internal::Seg.max_right(root, -linf, [&](long long x){ return x <= idx; });
    }
    long long at(int idx) { return (*this)[idx]; }
    long long front(){ return at(0); }
    long long back(){ return at(_size - 1); }
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