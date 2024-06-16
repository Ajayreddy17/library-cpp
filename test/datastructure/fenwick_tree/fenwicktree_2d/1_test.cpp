#include "library/template2.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_2d.hpp"
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
const int R = 100; // query / n ratio

const int X_S = 100;
const int Y_S = 200;
const long long X_L = 10000;
const long long Y_L = 20000;

void test_small(){
    int n = rand(Ln, Rn + 1);
    vector<int> X(n), Y(n);
    vector<long long> aux(n);

    For(x, n){
        X[x] = rand(-X_S, X_S);
        Y[x] = rand(-Y_S, Y_S);
        aux[x] = rand(-iinf, iinf);
    }

    FenwickTree_2D<Monoid_Add<long long>, int, true> Ft(X, Y, aux);
    
    int q = n * R;
    For(q){
        int i = rand(n);
        int A = rand(-iinf, iinf);

        aux[i] += A;
        Ft.add(X[i], Y[i], A);

        int x1 = rand(-X_S, X_S);
        int x2 = rand(x1, X_S + 1);
        int y1 = rand(-Y_S, Y_S);
        int y2 = rand(y1, Y_S + 1);

        long long prod = 0;
        For(x, n){
            if (X[x] >= x1 and X[x] < x2 and Y[x] >= y1 and Y[x] < y2){
                prod += aux[x];
            }
        }
        assert(prod == Ft.prod(x1, x2, y1, y2));
    }
}

void test_large(){
    int n = rand(Ln, Rn + 1);
    vector<long long> X(n), Y(n);
    vector<long long> aux(n);

    For(x, n){
        X[x] = rand(-X_L, X_L);
        Y[x] = rand(-Y_L, Y_L);
        aux[x] = rand(-iinf, iinf);
    }

    FenwickTree_2D<Monoid_Add<long long>, long long, false> Ft(X, Y, aux);
    
    int q = n * R;
    For(q){
        int i = rand(n);
        int A = rand(-iinf, iinf);

        aux[i] += A;
        Ft.add(X[i], Y[i], A);

        int x1 = rand(-X_L, X_L);
        int x2 = rand(x1, X_L + 1);
        int y1 = rand(-Y_L, Y_L);
        int y2 = rand(y1, Y_L + 1);

        long long prod = 0;
        For(x, n){
            if (X[x] >= x1 and X[x] < x2 and Y[x] >= y1 and Y[x] < y2){
                prod += aux[x];
            }
        }
        assert(prod == Ft.prod(x1, x2, y1, y2));
    }
}

int main(){
    
    const int TESTS = 10;
    print("Testing Fenwicktree_2D");
    For(x, TESTS) {
        test_small();
        printg("Passed small test:", x + 1);
    }
    For(x, TESTS) {
        test_large();
        printg("Passed large test:", x + 1);
    }

    printg("Fenwicktree_2D Passed All tests");
    return 0;
}