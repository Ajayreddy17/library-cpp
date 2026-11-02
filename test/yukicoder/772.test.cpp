#define PROBLEM "https://yukicoder.me/problems/no/772"
#include "library/template.hpp"
#include "library/graph/datastructure/link_cut_tree.hpp"
#include "library/datastructure/removable_queue.hpp"

struct Node {
    using np = Node *;
    struct VX {
        bool is_edge = false;
        long long x = 0;  // 頂点なら X[v], 辺なら長さ
    };
    // (wt, light edge の接続先)
    struct MX {
        Removable_Queue<max_priority_queue<pair<long long, int>>> que;
        long long sm = 0;
        long long dist_sum = 0;
    };

    struct X {
        long long wt_sum;
        long long length;      // heavy path len
        long long sum1, sum2;  // heavy path の端点からの dist sum
    };

    Node *l, *r, *p;
    int idx, size;  // size は heavy path の頂点数
    bool rev;
    VX vx;
    MX mx;
    X x;

    Node(int i = 0)
            : l(nullptr), r(nullptr), p(nullptr), idx(i), size(1), rev(0) {}

    void update() {
        size = 1;
        x.wt_sum = (vx.is_edge ? 0 : vx.x);
        x.wt_sum += mx.sm;
        if (l) {
            size += l->size;
            x.wt_sum += l->x.wt_sum;
        }
        if (r) {
            size += r->size;
            x.wt_sum += r->x.wt_sum;
        }

        long long l_len = (l ? l->x.length : 0);
        long long m_len = (vx.is_edge ? vx.x : 0);
        long long r_len = (r ? r->x.length : 0);
        x.length = l_len + m_len + r_len;
        x.sum1 = 0, x.sum2 = 0;
        x.sum1 += mx.sm * (l_len + m_len) + mx.dist_sum;
        x.sum2 += mx.sm * (r_len + m_len) + mx.dist_sum;
        x.sum1 += (vx.is_edge ? 0 : vx.x * l_len);
        x.sum2 += (vx.is_edge ? 0 : vx.x * r_len);
        if (l) {
            x.sum1 += l->x.sum1;
            x.sum2 += l->x.sum2 + (m_len + r_len) * l->x.wt_sum;
        }
        if (r) {
            x.sum2 += r->x.sum2;
            x.sum1 += r->x.sum1 + (m_len + l_len) * r->x.wt_sum;
        }
    }

    void push() {
        if (rev) {
            if (l) l->reverse();
            if (r) r->reverse();
            rev = 0;
        }
    }

    // data の reverse も行う
    void reverse() {
        rev ^= 1;
        swap(l, r);
        swap(x.sum1, x.sum2);
    }

    // LCT 内で expose, update を行うのでここは変更だけ
    void set(VX x) { vx = x; }

    // c がこの時点では update されていないかもしれないが, x は正常なものが入る
    // c->x 等は使わないように注意する
    // c->idx を持っておくと探索できることがある
    void add_light(Node *c) {
        auto x = c->x;
        mx.sm += x.wt_sum;
        mx.que.push(make_pair(x.wt_sum, c->idx));
        mx.dist_sum += x.sum1;
    }
    void erase_light(Node *c) {
        auto x = c->x;
        mx.sm -= x.wt_sum;
        mx.que.remove(make_pair(x.wt_sum, c->idx));
        mx.dist_sum -= x.sum1;
    }
    void change_light(np a, np b) {
        auto sm = b->x.wt_sum;
        mx.que.remove(make_pair(sm, a->idx));
        mx.que.push(make_pair(sm, b->idx));
    }
};

void solve() {
    LL(N, Q);
    Link_Cut_Tree<Node> LCT(N + Q);
    vector<long long> X(N, 1);
    For(i, N) { LCT.set(i, {false, 1}); }

    int pid = N;

    long long SUM_ANS = 0;
    auto read_vertex = [&]() -> long long {
        LL(x);
        return (x - 1 + SUM_ANS) % N;
    };

    auto solve_1 = [&](int a, int b, int c) -> void {
        int idx = pid++;
        LCT.set(idx, {true, c});
        LCT.link(a, idx), LCT.link(b, idx);
    };
    auto solve_2 = [&](int a, int b) -> void {
        int c = LCT.jump(a, b, 1);
        LCT.cut(a, c), LCT.cut(b, c);
    };
    auto solve_3 = [&](int a) -> long long {
        X[a] ^= 1;
        LCT.set(a, {false, X[a]});
        LCT.evert(a);
        long long total = LCT[a]->x.wt_sum;
        if (total == 0) return 0;
        long long half = cld<long long>(total, 2);
        // half 以上の重みを持つ subtree で topological 順で極大なところ

        using np = typename decltype(LCT)::np;
        auto dfs = [&](auto &dfs, np c, long long need) -> np {
            c->push();
            assert(c->x.wt_sum >= need);
            // long long a = (c->l ? c->l->x.wt_sum : 0);
            long long b = (c->r ? c->r->x.wt_sum : 0);
            long long me = (c->vx.is_edge ? 0 : c->vx.x);
            long long mid = c->mx.sm;
            if (me + mid + b >= need) {
                if (b >= need) {
                    return dfs(dfs, c->r, need);
                }
                if (mid >= half) {
                    auto [val, idx] = c->mx.que.top();
                    if (val >= half) return dfs(dfs, LCT[idx], half);
                }
                return c;
            }
            return dfs(dfs, c->l, need - (me + mid + b));
        };
        np c = dfs(dfs, LCT[a], half);
        LCT.evert(c);
        return c->x.sum1;
    };

    For(q, Q) {
        INT(t);
        if (t == 1) {
            long long a = read_vertex();
            long long b = read_vertex();
            LL(c);
            solve_1(a, b, c);
        }
        if (t == 2) {
            long long a = read_vertex();
            long long b = read_vertex();
            solve_2(a, b);
        }
        if (t == 3) {
            long long a = read_vertex();
            long long ans = solve_3(a);
            print(ans);
            SUM_ANS += ans;
            SUM_ANS %= N;
        }
    }
}

signed main() {
    solve();
    return 0;
}
