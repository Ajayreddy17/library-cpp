#ifndef AJAY_MULTISET
#define AJAY_MULTISET

#include "library/datastructure/segment_tree/dynamic_segtree_sparse.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
// Coded by AjayReddy.T
// Alternate for ordered multiset
template<typename T = long long>
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

    void insert(T x, int cnt = 1){
        _size += cnt;
        root = Meg.multiply(root, x, cnt);
    }
    void remove(T x, int cnt = 1){
        ASSERT(Meg.get(root, x) >= cnt);
        _size -= cnt;
        root = Meg.multiply(root, x, -cnt);
    }
    void erase(T x, int cnt = 1) { remove(x, cnt); }

    long long size(){ return _size; }

    optional<T> min_gt(T x){
        if (not Meg.prod(root, x + 1, linf)) return nullopt;
        return Meg.max_right(root, [](T x){ return not x; }, x + 1);
    }
    optional<T> min_geq(T x){ return min_gt(x - 1); }
    optional<T> max_lt(T x){
        if (not Meg.prod(root, -linf, x)) return nullopt;
        return Meg.min_left(root, [](T x){ return not x; }, x) - 1;
    }
    optional<T> max_leq(T x){ return min_lt(x + 1); }

    T count_gt(T x){ return Meg.prod(root, x + 1, linf); }
    T count_geq(T x){ return Meg.prod(root, x, linf); }
    T count_lt(T x){ return Meg.prod(root, -linf, x); }
    T count_leq(T x){ return Meg.prod(root, -linf, x + 1); }

    T count(T x){ return Meg.get(root, x); }
    // number of x s.t l <= x < r.
    T count_range(T l, T r){ return Meg.prod(root, l, r); }

    T starting_index(T x){
        ASSERT(count(x));
        return count_lt(x);
    }
    T ending_index(T x){
        ASSERT(count(x));
        return count_leq(x) - 1;
    }
    // index range
    pair<T, T> equal_range(T x){ return make_pair(count_lt(x), count_leq(x)); }

    T operator[](int idx) {
        ASSERT(idx < Meg.prod_all(root));
        return Meg.max_right(root, [&](T x){ return x <= idx; }, -linf);
    }
    T index(int idx) { return (*this)[idx]; }
    T front(){ return index(0); }
    T back(){ return index(_size - 1); }
#undef ASSERT
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const Multiset<T> &S){
    Multiset<T> M = S;
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