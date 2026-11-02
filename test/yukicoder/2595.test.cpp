#define PROBLEM "https://yukicoder.me/problems/no/2595"
#include "library/template.hpp"
#include "library/bigint/base.hpp"
#include "library/graph/tree.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_affine.hpp"

using B = BigInteger;
string S;
int N, p;

struct Node {
    char op;
    int L, R;
};

map<int, B> MP;
vector<Node> nodes;

int number();
int factor();
int term();
int expr();

int number() {
    debug("number", S.substr(p));
    bool neg = 0;
    while (S[p] == '-') neg ^= 1, ++p;
    int L = p;
    while (p < len(S) && ('0' <= S[p] && S[p] <= '9')) ++p;
    int R = p;
    B ans(S.substr(L, R - L));
    if (neg) ans = -ans;
    int n = len(nodes);
    nodes.emplace_back(Node{'.', -1, -1});
    MP[n] = ans;
    return n;
}

int factor() {
    debug("factor", S.substr(p));
    if (S[p] == '(') {
        ++p;
        int v = expr();
        assert(S[p] == ')');
        ++p;
        return v;
    }
    return number();
}

int term() {
    debug("term", S.substr(p));
    int v = factor();
    while (p < len(S) && S[p] == '*') {
        ++p;
        int w = factor();
        int n = len(nodes);
        nodes.emplace_back(Node{'*', v, w});
        v = n;
    }
    return v;
}

int expr() {
    debug("expr", S.substr(p));
    int v = term();
    while (p < N && (S[p] == '+' || S[p] == '-')) {
        char op = S[p];
        ++p;
        int w = term();
        int n = len(nodes);
        nodes.emplace_back(Node{op, v, w});
        v = n;
    }
    return v;
}

using Mono = Monoid_Affine<B>;
using F = Mono::value_type;

void solve() {
    read(N);
    read(S);
    assert(len(S) == N);
    int root = expr();
    assert(p == N);

    Graph<int, 1> G(root + 1);
    For(i, root + 1) {
        if (nodes[i].L == -1) continue;
        G.add(i, nodes[i].L);
        G.add(i, nodes[i].R);
    }
    G.build();

    Tree<decltype(G)> tree(G, root);

    auto dfs = [&](auto& dfs, int h) -> B {
        assert(tree.head[h] == h);
        auto path = tree.heavy_path_at(h);
        int n = len(path);
        vector<B> sub(n - 1);
        For(i, n - 1) {
            auto chs = tree.collect_light(path[i]);
            assert(len(chs) == 1);
            sub[i] = dfs(dfs, chs[0]);
        }
        SegTree<Mono> seg(n - 1, [&](int i) -> F {
            int v = path[n - 2 - i];
            B b = sub[n - 2 - i];
            int w = tree.heavy_child(v);
            if (nodes[v].op == '*') { return {b, 0}; }
            if (nodes[v].op == '+') { return {1, b}; }
            assert(nodes[v].op == '-');
            if (w == nodes[v].L) { return {1, -b}; }
            return {-1, b};
        });
        F f = seg.prod_all();
        return Mono::eval(f, MP[path[n - 1]]);
    };

    B ANS = dfs(dfs, root);
    print(ANS.to_string());
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
