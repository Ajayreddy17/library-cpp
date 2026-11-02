#define PROBLEM "https://yukicoder.me/problems/no/3229"
#include "library/template.hpp"
#include "library/datastructure/bit_array.hpp"
#include "library/mod/mod_pow.hpp"

using BS = Bit_Array;
int matrix_rank(vector<BS> a, int N, int M) {
    int rk = 0;
    For(j, M) {
        if (rk == N) break;
        if (!a[rk][j]) {
            For(i, rk + 1, N) if (a[i][j]) {
                swap(a[rk], a[i]);
                break;
            }
        }
        if (!a[rk][j]) continue;
        For(i, rk + 1, N) {
            if (a[i][j]) {
                a[i] ^= a[rk];
            }
        }
        ++rk;
    }
    return rk;
}

void solve() {
    LL(N, M, K);
    swap(N, M);
    bool bl = 0;
    vector<BS> a(N, BS(M));
    For(i, N) {
        STR(S);
        For(j, M) a[i][j] = (S[j] - '0');
    }

    long long ANS = mod_pow(2, M - matrix_rank(a, N, M), K);
    print(ANS);
}

signed main() { solve(); }
