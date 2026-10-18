#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <library/template.hpp>

#include <library/datastructure/segment_tree/segtree.hpp>
#include <library/monoid/monoid_add.hpp>

int main(){
    
    Int(n, q);
    Vt(long long, a, n);

    SegTree<Monoid_Add<long long>> Seg(a);
    Loop(q){
        Int(t);
        if (t == 0){
            Int(p, x);
            Seg[p] += x;
        }
        else{
            Int(l, r);
            print(Seg(l, r));
        }
    }

    return 0;
}