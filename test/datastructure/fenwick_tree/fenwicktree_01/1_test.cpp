#include "library/template2.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_01.hpp"
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
const int R = 100; 

void test(){
    int n = rand(Ln, Rn + 1);
    
    vector<int> aux(n);
    FenwickTree_01 Ft(n);

    int q = n * R;
    For(q){
        
        int idx = rand(n);
        aux[idx] ^= 1;

        if (aux[idx])
            Ft.add(idx);
        else   
            Ft.remove(idx);
        
        auto [l, r] = range(n);

        int cnt = 0;
        For(x, l, r) cnt += aux[x];
        assert(cnt == Ft.sum(l, r));

        int k = Ft.sum_all() / 2;
        int i = 0;
        while (i < n) {
            k -= aux[i];
            if (k < 0) break;
            i += 1;
        }
        assert(i == Ft.kth(Ft.sum_all() / 2));

        idx = rand(n);
        int idn = idx;
        while (idn < n and not aux[idn]) idn++;

        int idl = idx;
        while (idl >= 0 and not aux[idl]) idl--;
        assert(idl == Ft.prev(idx));
    }
}

int main(){
    
    const int TESTS = 10;
    print("Testing Fenwicktree01");
    For(x, TESTS) {
        test();
        printg("Passed test:", x + 1);
    }

    printg("Fenwicktree01 Passed All tests");
    return 0;
}