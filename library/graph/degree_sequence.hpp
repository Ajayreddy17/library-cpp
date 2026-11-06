#ifndef AJAY_DEGREE_SEQUENCE
#define AJAY_DEGREE_SEQUENCE

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/degree_sequence.hpp
// Determining the existence of a simple graph. Erdos-Gallai theorem.
bool check_degree_sequence(vector<int> deg) {
    int N = len(deg);
    if (N == 0) return true;
    long long sm = 0;
    vector<int> freq(N, 0);
    For(i, N) {
        int d = deg[i];
        if (!(0 <= d && d < N)) return false;
        freq[d]++, sm += d;
    }
    if (sm & 1) return false;
    int p = 0;
    Frr(x, N) For(freq[x]) deg[p++] = x;

    vector<long long> S(N + 1);
    For(i, N) S[i + 1] = S[i] + deg[i];
    long long m = 0; // # of d_i with d_i >= k
    Frr(k, N + 1) {
        while (m < N && deg[m] >= k) ++m;
        long long lhs = S[k];
        long long rhs = k * (k - 1);
        if (m < k) {
            rhs += S[N] - S[k];
        } else {
            rhs += (m - k) * k;
            rhs += S[N] - S[m];
        }
        if (lhs > rhs) return false;
    }
    return true;
}

// undirected Graph
vector<pair<int, int>> construct_from_degree_sequence(vector<int> deg) {
    if (!check_degree_sequence(deg)) return {};
    int N = len(deg);
    vector<vector<int>> dat(N);
    For(v, N) dat[deg[v]].emplace_back(v);
    vector<pair<int, int>> edges;
    int mx = N - 1;
    For(N) {
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