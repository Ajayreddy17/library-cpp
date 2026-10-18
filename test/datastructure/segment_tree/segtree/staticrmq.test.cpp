#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <library/template.hpp>

#include <library/datastructure/segment_tree/segtree.hpp>
#include <library/monoid/monoid_min.hpp>

int main(){
    
    Int(n, q);
    Vt(int, a, n);

    SegTree<Monoid_Min<int>> Seg(a);
    Loop(q){
        Int(l, r);
        print(Seg(l, r));
    }

    return 0;
}