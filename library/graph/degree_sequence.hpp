#ifndef AJAY_DEGREE_SEQUENCE
#define AJAY_DEGREE_SEQUENCE

namespace mitsuha{
// Determining the existence of a simple graph. Erdos-Gallai theorem.
bool check_degree_sequence(vector<int> deg) {
    const int N = len(deg);
    if (N == 0) return true;
    if (*max_element(deg.begin(), deg.end()) >= N) return false;
    if (accumulate(deg.begin(), deg.end(), 0LL) % 2 != 0) return false;
    vector<int> CNT(N);
    for (auto&& x: deg) CNT[x]++;
    int p = 0;
    For(x, N) Loop(CNT[x]) deg[p++] = x;

    vector<long long> A(N + 1), B(N + 1);
    For(i, N) {
        int d = deg[i];
        A[i + 1] += 2 * i - d;
        if (d < i) { B[0] += 1, B[d] -= 1, A[d] += d, A[i + 1] -= d; }
        if (d >= i) { B[0] += 1, B[i + 1] -= 1; }
    }
    partial_sum(A.begin(), A.end(), A.begin());
    partial_sum(B.begin(), B.end(), B.begin());
    For(k, N + 1) {
        long long x = A[k] + B[k] * k;
        if (x < 0) return false;
    }
    return true;
}

vector<pair<int, int>> construct_from_degree_sequence(vector<int> deg) {
    if (!check_degree_sequence(deg)) return {};
    int N = len(deg);
    vector<vector<int>> dat(N);
    For(v, N) dat[deg[v]].emplace_back(v);
    vector<pair<int, int>> edges;
    int mx = N - 1;
    Loop(N) {
        while (mx >= 0 && len(dat[mx]) == 0) --mx;
        int v = dat[mx].back(); dat[mx].pop_back();
        vector<int> nbd;
        int k = mx;
        while (len(nbd) < deg[v]) {
            if (k == 0) return {};
            if (len(dat[k]) == 0) {
                --k;
                continue;
            }
            int x = dat[k].back(); dat[k].pop_back();
            nbd.emplace_back(x);
        }
        for (auto&& x: nbd) {
            edges.emplace_back(v, x);
            --deg[x];
            dat[deg[x]].emplace_back(x);
        }
        deg[v] = 0;
    }
    return edges;
}
} // namespace mitsuha
#endif // AJAY_DEGREE_SEQUENCE