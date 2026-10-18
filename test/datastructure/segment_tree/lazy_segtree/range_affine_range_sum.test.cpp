#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/datastructure/segment_tree/lazy_segtree.hpp>
#include <library/acted_monoid/am_sum_affine.hpp>

using mint = modint998;

int main(){
    INT(n, q);

    VT(mint, a, n);
    Lazy_SegTree<ActedMonoid_Sum_Affine<mint>> Lseg(a);
    For(q){
        INT(t);
        if (t == 0){
            INT(l, r);
            mint b, c;
            read(b, c);

            Lseg.apply(l, r, {b, c});
        }
        else{
            INT(l, r);
            print(Lseg(l, r));
        }
    }
    return 0;
}
