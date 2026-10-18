#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"

#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/datastructure/segment_tree/dual_segtree.hpp>
#include <library/monoid/monoid_affine.hpp>

using mint = modint998;

int main(){
    Int(n, q);

    Vt(mint, a, n);
    Dual_SegTree<Monoid_Affine<mint>> Dseg(n);
    Loop(q){
        Int(t);
        if (t == 0){
            Int(l, r);
            mint b, c;
            read(b, c);

            Dseg.apply(l, r, {b, c});
        }
        else{
            Int(p);
            print(Monoid_Affine<mint>::eval(Dseg[p], a[p]));
        }
    }
    return 0;
}