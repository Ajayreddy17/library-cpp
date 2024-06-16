#include "library/template2.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_2d_dense.hpp"
#include "library/random/util.hpp"

#define printr(...) cout << "\033[31m", print(__VA_ARGS__, "\033[0m")
#define printg(...) cout << "\033[32m", print(__VA_ARGS__, "\033[0m")
  
void help(bool x){
    if (not x){
        printr("Failed");
        assert(0);
    }
}
#define assert(x) help(x)

const int Ln = (int) 100;
const int Rn = (int) 2000;
const int R = 40; 

void test(){
    int n = rand(Ln, Rn + 1);
    int m = rand(Ln, Rn + 1);

    auto aux = ndvec(Dim(n, m), 0LL);

    For(x, n){
        For(y, m){
            aux[x][y] = rand(-iinf, iinf);
        }
    }
    FenwickTree_2D_Dense<Monoid_Add<long long>> Ft(aux);

    int q = max(n, m) * R;

    For(q){
        int ix = rand(n);
        int iy = rand(m);
        int v = rand(-iinf, iinf);

        Ft.add(ix, iy, v);
        aux[ix][iy] += v;

        auto [x1, x2] = range(n);
        auto [y1, y2] = range(m);

        long long prod = 0;
        For(x, x1, x2){
            For(y, y1, y2){
                prod += aux[x][y];
            }
        }
        assert(prod == Ft.prod(x1, x2, y1, y2));
    }
    For(x, n) For(y, m) assert(Ft.get(x, y) == aux[x][y]);
}

int main(){
    
    const int TESTS = 10;
    print("Testing Fenwicktree_2d_dense");
    For(x, TESTS) {
        test();
        printg("Passed test:", x + 1);
    }

    printg("Fenwicktree_2d_dense Passed All tests");
    return 0;
}