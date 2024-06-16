#include "library/template2.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree.hpp"
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

const int Ln = (int) 10;
const int Rn = (int) 10000;
const int R = 40; 

void test(){
    int n = rand(Ln, Rn + 1);
    vector<long long> aux(n);

    For(x, n){
        aux[x] = rand(0, iinf);
    }
    FenwickTree<Monoid_Add<long long>> Ft(aux);

    int q = n * R;

    For(q){
        int i = rand(n);
        int v = rand(0, iinf);

        aux[i] += v;
        Ft[i] += v;

        auto [l, r] = range(n);
        long long prod = 0;
        For(x, l, r){
            prod += aux[x];
        }
        assert(prod == Ft(l, r));

        int idx = rand(n);
        long long obj = rand(1, iinf) * rand(1, n + 1 - idx);
        int idr = idx;
        prod = 0;
        while (idr < n and prod + aux[idr] < obj) {
            prod += aux[idr];
            idr += 1;
        }
        assert(idr == Ft.max_right(idx, [&](long long E){
            return E < obj;
        }));

        int idl = idx;
        prod = 0;
        while (idl - 1 >= 0 and prod + aux[idl - 1] < obj){
            prod += aux[idl - 1];
            idl -= 1;
        }
        assert(idl == Ft.min_left(idx, [&](long long E){
            return E < obj;
        }));
        assert(aux == Ft.get_all());
    }
    For(x, n){
        assert(aux[x] == Ft[x]);
    }
}

int main(){
    
    const int TESTS = 10;
    print("Testing Fenwicktree");
    For(x, TESTS) {
        test();
        printg("Passed test:", x + 1);
    }

    printg("Fenwicktree Passed All tests");
    return 0;
}