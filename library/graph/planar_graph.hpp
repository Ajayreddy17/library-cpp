#ifndef AJAY_PLANAR_GRAPH
#define AJAY_PLANAR_GRAPH

#include "library/graph/base.hpp"
#include "library/geo/base.hpp"
#include "library/geo/angle_sort.hpp"

namespace mitsuha{
template <typename XY>
struct Planar_Graph {
    using P = Point<XY>;
    int NV, NE, NF;
    Graph<int, 1> G;
    vector<P> point;
    vector<int> left_face;
    vector<int> nxt_edge;
    vector<int> first_edge;

    Planar_Graph(int N, vector<P> point) : NV(N), G(N), point(point) { assert(N > 1); }

    void add(int a, int b) { G.add(a, b), G.add(b, a); }
    void build() {
        G.build();
        NE = G.M / 2;
        nxt_edge.assign(G.M, -1);
        left_face.assign(G.M, -1);
        int v0 = 0;
        int e0 = 0;
        For(v, NV) {
            if (point[v] < point[v0]) v0 = v;
            vector<int> eid;
            vector<P> dir;
            for (auto& e: G[v]) {
                eid.emplace_back(e.id);
                dir.emplace_back(point[e.to] - point[e.frm]);
            }
            auto I = angle_sort(dir);
            assert(len(I) > 0);
            For(k, len(I)) {
                int i = (k == 0 ? I.back() : I[k - 1]);
                int j = I[k];
                i = eid[i], j = eid[j];
                nxt_edge[j ^ 1] = i;
            }
            if (v == v0) e0 = eid[I[0]] ^ 1;
        }
        for (auto& x: nxt_edge) assert(x != -1);

        auto make_face = [&](int e) -> void {
            int p = len(first_edge);
            first_edge.emplace_back(e);
            while (left_face[e] == -1) {
                left_face[e] = p;
                e = nxt_edge[e];
            }
        };

        make_face(e0);
        For(e, 2 * NE) {
            if (left_face[e] == -1) make_face(e);
        }
        NF = len(first_edge);
        assert(NV - NE + NF == 2);
    }

    pair<vector<int>, vector<int>> get_face_data(int fid) {
        vector<int> eid = {first_edge[fid]};
        while (1) {
            int e = nxt_edge[eid.back()];
            if (e == first_edge[fid]) break;
            eid.emplace_back(e);
        }
        vector<int> vid;
        for (auto& e: eid) vid.emplace_back(G.edges[e].frm);
        vid.emplace_back(vid[0]);
        return {vid, eid};
    }
};
} // namespace mitsuha
#endif // AJAY_PLANAR_GRAPH
