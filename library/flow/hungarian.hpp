#ifndef AJAY_HUNGARIAN
#define AJAY_HUNGARIAN

namespace mitsuha{
// returns: (ans, match, X, Y)
// N <= M Assuming, infinity is also considered valid in the calculation, O(N^2M) time。
// The potential is the solution to the following dual problem：
// maximize \sum x_i + \sum y_j, subj to x_i + y_j\leq C_{ij}
// n Even if the loop is terminated, it is not the optimal solution for a matching of size n
template <typename T, bool MINIMIZE>
tuple<T, vector<int>, vector<T>, vector<T>> hungarian(vector<vector<T>>& C) {
    if (C.empty()) { return {0, {}, {}, {}}; }
    int N = C.size();
    int M = C[0].size();
    assert(N <= M);
    vector<vector<T>> A(N + 1, vector<T>(M + 1));
    For(i, N) For(j, M) A[1 + i][1 + j] = (MINIMIZE ? 1 : -1) * C[i][j];
    ++N, ++M;

    vector<int> P(M), way(M);
    vector<T> X(N), Y(M);
    vector<T> minV;
    vector<bool> used;

    for (int i = 1; i < N; i++) {
        P[0] = i;
        minV.assign(M, numeric_limits<T>::max() / 2);
        used.assign(M, false);
        int j0 = 0;
        while (P[j0] != 0) {
            int i0 = P[j0], j1 = 0;
            used[j0] = true;
            T delta = numeric_limits<T>::max() / 2;
            for (int j = 1; j < M; j++) {
                if (used[j]) continue;
                T curr = A[i0][j] - X[i0] - Y[j];
                if (curr < minV[j]) 
                    minV[j] = curr, way[j] = j0;
                if (minV[j] < delta)
                    delta = minV[j], j1 = j;
            }
            for (int j = 0; j < M; j++) {
                if (used[j]) X[P[j]] += delta, Y[j] -= delta;
                else minV[j] -= delta;
            }
            j0 = j1;
        }
        do {
            P[j0] = P[way[j0]];
            j0 = way[j0];
        } while (j0 != 0);
    }
    T res = -Y[0];
    X.erase(X.begin());
    Y.erase(Y.begin());
    vector<int> match(N);
    For(i, N) match[P[i]] = i;
    match.erase(match.begin());
    for (auto&& i : match) --i;
    if (!MINIMIZE) res = -res;
    return {res, match, X, Y};
}
} // namespace mitsuha
#endif // AJAY_HUNGARIAN