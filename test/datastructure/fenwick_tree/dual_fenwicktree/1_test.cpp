#include "library/template2.hpp"
#include "library/datastructure/fenwick_tree/dual_fenwicktree.hpp"
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
const int R = 5; // query / n ratio

void test(){
    int n = rand(Ln, Rn + 1);
    Dual_FenwickTree<Monoid_Add<long long>> Df(n);

    vector<long long> aux(n);
    int q = 5 * n;

    For(q){
        auto [l, r] = range(n);
        long long A = rand(-iinf, iinf);

        Df.apply(l, r, A);
        For(x, l, r){
            aux[x] += A;
        }
    }
    For(x, n){
        assert(aux[x] == Df[x]);
    }
    assert(aux == Df.get_all());
}

int main(){
    
    const int TESTS = 10;
    print("Testing Dual_Fenwicktree");
    For(x, TESTS) {
        test();
        printg("Passed test:", x + 1);
    }

    printg("Dual_Fenwicktree Passed All tests");
    return 0;
}