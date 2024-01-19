#ifndef AJAY_TRANSPOSE
#define AJAY_TRANSPOSE

namespace mitsuha{
template <typename VC>
vector<VC> transpose(const vector<VC>& A, int H = -1, int W = -1) {
    if (H == -1) { H = len(A), W = len(A[0]); }
    vector<VC> B(W, VC(H, A[0][0]));
    For(x, H) For(y, W) B[y][x] = A[x][y];
    return B;
}
} // namespace mitsuha
#endif // AJAY_TRANSPOSE