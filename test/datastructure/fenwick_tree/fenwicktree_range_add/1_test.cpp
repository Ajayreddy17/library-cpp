#include "library/template2.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_range_add.hpp"
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
const int Rn = (int) 10000;
const int R = 40; // query / n ratio

void test(){
    int n = rand(Ln, Rn + 1);
    vector<long long> aux(n);
    For(x, n) aux[x] = rand(-iinf, iinf);

    FenwickTree_Range_Add<Monoid_Add<long long>> Ft(aux);

    int q = R * n;
    For(q){
        auto [l, r] = range(n);
        long long A = rand(-iinf, iinf);
        Ft.add(l, r, A);
        For(x, l, r){
            aux[x] += A;
        }

        tie(l, r) = range(n);
        long long prod = 0;
        For(x, l, r) prod += aux[x];

        assert(prod == Ft.prod(l, r));
    }
}

int main(){
    
    const int TESTS = 10;
    print("Testing Fenwicktree_Range_Add");
    For(x, TESTS) {
        test();
        printg("Passed test:", x + 1);
    }

    printg("Fenwicktree_Range_Add Passed All tests");
    return 0;
}