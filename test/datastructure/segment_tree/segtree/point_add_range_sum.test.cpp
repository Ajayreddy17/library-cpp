#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <library/template.hpp>

#include <library/datastructure/segment_tree/segtree.hpp>
#include <library/monoid/monoid_add.hpp>

int main(){

    INT(n, q);
    VT(long long, a, n);

    SegTree<Monoid_Add<long long>> Seg(a);
    For(q){
        INT(t);
        if (t == 0){
            INT(p, x);
            Seg[p] += x;
        }
        else{
            INT(l, r);
            print(Seg(l, r));
        }
    }

    return 0;
}
