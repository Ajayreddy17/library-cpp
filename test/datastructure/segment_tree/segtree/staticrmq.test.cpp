#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <library/template.hpp>

#include <library/datastructure/segment_tree/segtree.hpp>
#include <library/monoid/monoid_min.hpp>

int main(){

    INT(n, q);
    VT(int, a, n);

    SegTree<Monoid_Min<int>> Seg(a);
    For(q){
        INT(l, r);
        print(Seg(l, r));
    }

    return 0;
}
