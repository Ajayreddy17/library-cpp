#ifndef AJAY_EULERIAN
#define AJAY_EULERIAN

#include "library/mod/modint.hpp"
#include "library/linear_algebra/det.hpp"
#include "library/graph/base.hpp"

namespace mitsuha{
template<class S>
vector<S> count_outdeg(vector<vector<S>> &G){
    int N = G.size();
    vector<S> outdeg(N);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) outdeg[i] += G[i][j];
    return outdeg;
}

template<class S>
vector<S> count_indeg(vector<vector<S>> &G){
    int N = G.size();
    vector<S> indeg(N);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) indeg[j] += G[i][j];
    return indeg;
}

template<typename mint, typename S>
vector<vector<mint>> Directed_Matrix_tree_Theorem_sub(vector<vector<S>> &G, int u = 0){
    int N = G.size();
    vector L(N - 1, vector<mint>(N - 1, 0));
    vector<S> outdeg(N);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) if (i != j) outdeg[i] += G[i][j];
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++){
            if (i == u || j == u) continue;
            int a = i, b = j;
            if (u < i) a--;
            if (u < j) b--;
            if (i == j) L[a][b] = outdeg[i];
            else L[a][b] -= G[i][j];
        }
    return L;
}

template<typename mint, typename S>
mint Directed_Matrix_tree_Theorem(vector<vector<S>> &G, int u = 0){
    if (int(G.size()) == 1) return mint(1);
    return det<mint>(Directed_Matrix_tree_Theorem_sub<mint>(G, u));
}

// s.t
// forall i,j
// 0 <= G[i][j]
// if not connected
// use remove iso
template<typename mint, typename S>
pair<mint, vector<vector<mint>>> Count_Euler_Circuit_sub(vector<vector<S>> G, bool fact_in = true){
    int N = G.size();
    if (N == 0) return {1, {{1}}};
    vector<S> outdeg = count_outdeg(G);
    vector<S> indeg  = count_indeg(G);

    // indeg  == outdeg ?
    for (int i = 0; i < N; i++) if (indeg[i] != outdeg[i]) return {0, {{1}}};

    // connected ?
    vector<bool> seen(N);
    vector<int> order={0};
    seen[0] = true;
    for (int i = 0; i < N; i++){
        if (i == int(order.size())) return {0, {{1}}};
        for (int j = 0; j < N; j++){
            if (G[order[i]][j] != 0 && !seen[j]){
                seen[j] = true;
                order.push_back(j);
            }
        }
    }
    mint res = 1;
    if (fact_in) for (int i = 0; i < N; i++) res *= fact<mint>(outdeg[i] - 1);
    return {res ,Directed_Matrix_tree_Theorem_sub<mint, S>(G)};
}

template<typename mint, typename S>
mint Count_Euler_Circuit(vector<vector<S>> G, bool fact_in = true){
    auto tmp = Count_Euler_Circuit_sub(G, fact_in);
    if (tmp.first == 0) return mint(0);
    return tmp.first * det(tmp.second);
}

template<class S>
vector<vector<S>> remove_isolated_vertex(vector<vector<S>> G){
    int N = G.size();
    vector<int> seen(N, -1);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++){
        if (G[i][j] != 0) seen[i] = 1, seen[j] = 1;
    }
    int ind = 0;
    for (int i = 0; i < N; i++) if (seen[i] == 1) seen[i] = ind++;
    vector res(ind, vector<S>(ind));
    for (int i = 0; i < N; i++) if (seen[i] != -1){
        for (int j = 0; j < N; j++) if (seen[j] != -1){
            res[seen[i]][seen[j]] = G[i][j];
        }
    }
    return res;
}

// s.t
// forall i,j
// 0 <= G[i][j]
// if not connected
// use remove iso
template<typename mint, typename S>
pair<mint, vector<vector<mint>>> Count_Eulerian_Trail_sub(vector<vector<S>> G, bool fact_in = true){
    bool ok = 1;
    int N = G.size();
    if (N == 0) return {1, {{1}}};
    vector<S> outdeg = count_outdeg(G), indeg = count_indeg(G);
    S sum = 0;
    for (int i = 0; i < N; i++){
        sum += outdeg[i];
    }
    int st = -1, ed = -1;
    for (int i = 0; i < N; i++) if (outdeg[i] != indeg[i]){
            if (abs(outdeg[i] - indeg[i]) > 1){
                ok = 0;
                break;
            }
            if (outdeg[i] > indeg[i]){
                if (ed != -1) ok = 0;
                ed = i;
            } else {
                if (st != -1) ok = 0;
                st = i;
            }
        }
    if (!ok) return {0, {{1}}};
    if ((st == -1) ^ (ed == -1)) return {0, {{1}}};
    if (st == -1){
        auto tmp = Count_Euler_Circuit_sub<mint>(G, fact_in);
        return {mint(sum) * tmp.first, tmp.second};
    }
    G[st][ed]++;
    return Count_Euler_Circuit_sub<mint>(G, fact_in);
}

template<typename mint, typename S>
mint Count_Eulerian_Trail(std::vector<std::vector<S>> G, bool fact_in = true){
    auto tmp = Count_Eulerian_Trail_sub<mint, S>(G, fact_in);
    if (tmp.first == 0) return 0;
    return tmp.first * det(tmp.second);
}

template<typename GT>
vector<vector<typename GT::cost_type>> graph_to_adj_matrix(GT &G){
    int N = G.N;
    using S = GT::cost_type;
    vector<vector<S>> M(N, vector<S>(N, 0));
    for (auto &&ed: G.edges){
        M[ed.frm][ed.to] = ed.cost;
    }
    return M;
}
} // namespace mitsuha
#endif // AJAY_EULERIAN