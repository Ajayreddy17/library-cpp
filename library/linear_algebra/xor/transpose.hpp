#ifndef AJAY_XOR_TRANSPOSE
#define AJAY_XOR_TRANSPOSE

namespace mitsuha{
// Transpose of an n x m matrix. O((n+m)log(n+m)) time.
// https://github.com/dsnet/matrix-transpose
template <typename UINT>
vector<UINT> transpose(int n, int m, vector<UINT>& A, bool keep_A = 1) {
    assert(max(n, m) <= numeric_limits<UINT>::digits);
    assert(len(A) == n);
    vector<UINT> tmp;
    if (keep_A) tmp = A;
    int LOG = 0;
    while ((1 << LOG) < max(n, m)) ++LOG;
    A.resize(1 << LOG);
    int width = 1 << LOG;
    UINT mask = 1;
    for(int i = 0; i < LOG; ++i) mask = mask | (mask << (1 << i));
    for(int t = 0; t < LOG; ++t) {
        width >>= 1;
        mask = mask ^ (mask >> width);
        for(int i = 0; i < (1 << t); ++i) {
            for(int j = 0; j < width; ++j) {
                UINT* x = &A[width * (2 * i + 0) + j];
                UINT* y = &A[width * (2 * i + 1) + j];
                *x = ((*y << width) & mask) ^ *x;
                *y = ((*x & mask) >> width) ^ *y;
                *x = ((*y << width) & mask) ^ *x;
            }
        }
    }
    A.resize(m);
    if (!keep_A) return A;
    swap(A, tmp);
    return tmp;
}
} // namespace mitsuha
#endif // AJAY_XOR_TRANSPOSE