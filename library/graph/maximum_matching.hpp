#ifndef AJAY_MAXIMUM_MATCHING
#define AJAY_MAXIMUM_MATCHING

#include "library/graph/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/maximum_matching.hpp

// return : (match size, match)
// match[v] : partner of v, or 0
// O(N^3)
// Korte-Vygen, Combinatorial Optimization, 2nd ed., Algorithm 10.2
template <typename GT>
pair<int, vector<int>> maximum_matching(const GT& G) {
    const int N = G.N;
    vector<int> mu(N), phi(N), rho(N);
    vector<bool> scanned(N);
    For(v, N) mu[v] = v;
    long long ans = 0;
    for (auto&& e: G.edges) {
        if (e.frm != e.to && mu[e.frm] == e.frm && mu[e.to] == e.to) {
            mu[e.frm] = e.to, mu[e.to] = e.frm, ++ans;
        }
    }

    auto odd = [&](int x) -> bool {
        return mu[x] != x && phi[mu[x]] == mu[x] && mu[x] != x;
    };
    auto out_of_forest = [&](int x) -> bool {
        return mu[x] != x && phi[mu[x]] == mu[x] && phi[x] == x;
    };
    auto P = [&](int x) -> vector<int> {
        vector<int> P;
        P.emplace_back(x);
        while (mu[x] != x) {
            P.emplace_back(mu[x]);
            P.emplace_back(phi[mu[x]]);
            x = phi[mu[x]];
        }
        return P;
    };

    vector<bool> on_path(N);
    while (1) {
        For(v, N) phi[v] = rho[v] = v, scanned[v] = 0;
        bool aug = 0;
        while (1) {
            bool upd = 0;
            For(x, N) {
                if (upd) break;
                if (scanned[x] || odd(x)) continue;
                for (auto&& e: G[x]) {
                    int y = e.to;
                    if (out_of_forest(y)) {
                        upd = 1;
                        // grow
                        phi[y] = x;
                    }
                    elif (rho[y] != rho[x] && !odd(y)) {
                        vector<int> F;
                        For(v, N) if (!out_of_forest(v)) F.emplace_back(v);
                        upd = 1;
                        // augument OR shrink
                        vector<int> Px = P(x);
                        vector<int> Py = P(y);
                        if (Px.back() != Py.back()) {
                            aug = 1;
                            // augument
                            For(2) {
                                swap(Px, Py);
                                for (int i = 1; i < len(Px); i += 2) {
                                    int v = Px[i];
                                    mu[phi[v]] = v, mu[v] = phi[v];
                                }
                            }
                            mu[x] = y, mu[y] = x, ++ans;
                            break;
                        } else {
                            // shrink
                            int r = -1;
                            int Nx = len(Px), Ny = len(Py);
                            for (int i = 0; i < Nx; i += 2) {
                                int v = Px[i];
                                int j = i + Ny - Nx;
                                if (0 <= j && j < Ny && Py[j] == v && rho[v] == v) {
                                    r = v;
                                    break;
                                }
                            }
                            while (Px.back() != r) Px.pop_back();
                            while (Py.back() != r) Py.pop_back();
                            vector<int> change;
                            For(2) {
                                swap(Px, Py);
                                for (int i = 1; i < len(Px); i += 2) {
                                    int v = Px[i];
                                    if (rho[phi[v]] != r) change.emplace_back(v);
                                }
                            }
                            for (auto&& v: change) phi[phi[v]] = v;

                            if (rho[x] != r) phi[x] = y;
                            if (rho[y] != r) phi[y] = x;
                            for (auto&& v: Px) on_path[v] = 1;
                            for (auto&& v: Py) on_path[v] = 1;
                            For(v, N) if (on_path[rho[v]]) { rho[v] = r; }
                            fill(on_path.begin(), on_path.end(), 0);
                        }
                    }
                }
                scanned[x] = 1;
            }
            if (!upd || aug) break;
        }
        if (!aug) break;
    }
    For(v, N) if (mu[v] == v) mu[v] = -1;
    return {ans, mu};
}
} // namespace mitsuha
#endif // AJAY_MAXIMUM_MATCHING
