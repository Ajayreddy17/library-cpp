#include "library/template2.hpp"
#include "library/datastructure/triangular_sum_2d.hpp"
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
const int Rn = (int) 1000;
const int R = 5; 

void test(){
    int n = rand(Ln, Rn + 1);
    int m = rand(Ln, Rn + 1);

    auto aux = ndvec(Dim(n, m), 0LL);
    For(x, n){
        For(y, m) aux[x][y] = rand(-iinf, iinf);
    }

    Triangular_Sum_2D<Monoid_Add<long long>> Ft(aux);

    auto in = [&](int x, int y){
        return bool(x >= 0 and x < n and y >= 0 and y < m);
    };

    int q = R * max(n, m);
    For(q){

        int k = rand(max(n, m));
        int ix = rand(n);
        int iy = rand(m);

        long long upper_left = 0;
        long long lower_left = 0;
        long long upper_right = 0;
        long long lower_right = 0;
        For(x, ix - k, ix){
            For(y, iy - (x - (ix - k) + 1), iy){
                if (x >= 0 and x < n and y >= 0 and y < m) upper_left += aux[x][y];
                if (in(2 * ix - x - 1, y))              lower_left += aux[2 * ix - x - 1][y];
                if (in(x, 2 * iy - y - 1))              upper_right += aux[x][2 * iy - y - 1];
                if (in(2 * ix - x - 1, 2 * iy - y - 1)) lower_right += aux[2 * ix - x - 1][2 * iy - y - 1];
            }
        }
        assert(upper_left == Ft.upper_left(ix, iy, k));
        assert(upper_right == Ft.upper_right(ix, iy, k));
        assert(lower_left == Ft.lower_left(ix, iy, k));
        assert(lower_right == Ft.lower_right(ix, iy, k));
    }
}

int main(){
    
    const int TESTS = 10;
    print("Testing Triangular Fenwicktree_2D");
    For(x, TESTS) {
        test();
        printg("Passed test:", x + 1);
    }

    printg("Triangular Fenwicktree_2D Passed All tests");
    return 0;
}