#ifndef AJAY_SLOPE_TRICK_SUPER
#define AJAY_SLOPE_TRICK_SUPER

#include "library/datastructure/splay_tree/splay_tree.hpp"
#include "library/monoid/monoid_add_pair.hpp"

namespace mitsuha{
namespace SLOPE_TRICK_SUPER {
/*
The slope and coordinates are all T.
(x0, y0, a0) / The slope change is stored in a splay tree.
Make sure that infinity is always at the end.
(0,10), (1,6), (3,4), (6,7)
->
(x0, y0, a0) = (0,10,-4)
dat = ([1,3], [3,2])

Calculating f(x), (min, argmin) calculation
Addition, Convolution

Addition: easy
Calculating f(x): sum(a), sum(xa) are required
Convolution: x -> x + c is required
*/

template <typename T>
struct Node {
    using value_type = pair<T, T>;
    using operator_type = T;
    using np = Node *;
    using Monoid_X = Monoid_Add_Pair<T>;

    np p, l, r;
    bool rev;
    unsigned int size;
    pair<T, T> x;    // (x,a)
    pair<T, T> prod; // (a sum, xa sum)
    T add_x;

    static void new_node(np n, const pair<T, T> &x) {
        n->p = n->l = n->r = nullptr, n->rev = 0, n->size = 1;
        n->x = x, n->prod = {x.second, x.first * x.second}, n->add_x = 0;
    }

    void update() {
        size = 1;
        if (l) { size += l->size; }
        if (r) { size += r->size; }
        prod = {x.second, x.first * x.second};
        if (l) prod = Monoid_X::op(prod, l->prod);
        if (r) prod = Monoid_X::op(prod, r->prod);
    }

    void prop() {
        assert(!rev);
        if (add_x == 0) return;
        if (l) l->x.first += add_x, l->prod.second += l->prod.first * add_x, l->add_x += add_x;
        if (r) r->x.first += add_x, r->prod.second += r->prod.first * add_x, r->add_x += add_x;
        add_x = 0;
    }

    void apply(T a) { x.first += a, prod.second += a * prod.first, add_x += a; }

    pair<T, T> get() { return x; }
    void set(const pair<T, T> &xx) {
        x = xx;
        update();
    }
};

// The function will be subject to destructive changes
template <typename T>
struct Slope_Trick_Super {
    SplayTree<Node<T>> ST;
    using np = Node<T> *;

    struct FUNC {
        np root; // If the domain is broken. root == empty
        T x0, x1, a0, y0;
        int size() { return (root ? root->size : 0); }
    };

    Slope_Trick_Super(int NODES) : ST(NODES) {}

    // (L,R,a,b) : [L,R] with y=ax+b
    FUNC segment_func(T L, T R, T a, T b) { return {nullptr, L, R, a, a * L + b}; }
    FUNC from_points(vector<pair<T, T>> &point) {
        return from_points(len(point), [&](int i) -> pair<T, T> { return point[i]; });
    }
    template <typename F>
    FUNC from_points(int N, F f) {
        vector<T> X(N), Y(N);
        For(i, N) tie(X[i], Y[i]) = f(i);
        if (N == 1)
            return segment_func(X[0], X[0], 0, Y[0]);
        T a0 = (Y[1] - Y[0]) / (X[1] - X[0]);
        T x0 = X[0], x1 = X.back();
        vector<pair<T, T>> dat;
        T a = a0;
        For(i, 1, N - 1) {
            T a1 = (Y[i + 1] - Y[i]) / (X[i + 1] - X[i]);
            dat.emplace_back(X[i], a1 - a), a = a1;
        }
        return FUNC{ST.new_node(dat), x0, x1, a0, Y[0]};
    }

    pair<T, T> domain(FUNC &f) { return {f.x0, f.x1}; }
    T eval(FUNC &f, T x) {
        auto [x0, x1] = domain(f);
        if (!(x0 <= x && x <= x1))
            return numeric_limits<T>::max() / 2;
        auto [l, r] = ST.split_max_right(f.root, [&](auto dat) -> bool { return dat.first <= x; });
        auto [a_sum, xa_sum] = ST.prod(l);
        f.root = ST.merge(l, r);
        return f.y0 + f.a0 * (x - x0) + a_sum * x - xa_sum;
    }
    FUNC restrict_domain(FUNC &f, T L, T R) {
        auto [x0, x1] = domain(f);
        chmax(L, x0), chmin(R, x1);
        if (L > R) {
            ST.free_subtree(f.root), f.root = nullptr;
            f.root = nullptr;
            f.x0 = numeric_limits<T>::max() / 2, f.x1 = -numeric_limits<T>::max() / 2;
            return f;
        }
        // First, shrink the right side. You can eliminate any slope changes greater than or equal to R
        auto [l, r] = ST.split_max_right(f.root, [&](auto dat) -> bool { return dat.first < R; });
        ST.free_subtree(r);
        // Shrink the left side.
        tie(l, r) = ST.split_max_right(l, [&](auto dat) -> bool { return dat.first <= L; });
        auto [a_sum, xa_sum] = ST.prod(l);
        T new_a0 = f.a0 + a_sum;
        T new_y0 = f.y0 + f.a0 * (L - x0) + a_sum * L - xa_sum;
        ST.free_subtree(l);
        f.root = r, f.x0 = L, f.x1 = R, f.a0 = new_a0, f.y0 = new_y0;
        return f;
    }
    FUNC add(FUNC &f, FUNC &g) {
        T x0 = max(f.x0, g.x0);
        T x1 = min(f.x1, g.x1);
        restrict_domain(f, x0, x1), restrict_domain(g, x0, x1);
        if (x0 > x1) return f;
        T y0 = f.y0 + g.y0, a0 = f.a0 + g.a0;

        if (len(f) < len(g)) swap(f, g);
        auto tmp = ST.get_all(g.root);
        ST.free_subtree(g.root);
        f.x0 = x0, f.x1 = x1, f.a0 = a0, f.y0 = y0;
        if (!f.root) {
            f.root = ST.new_node(tmp);
            return f;
        }
        // After that, you can simply insert tmp
        auto dfs = [&](auto &dfs, np root, int l, int r) -> void {
            if (l == r)
                return;
            root->prop();
            T x = root->x.first;
            // [l,m),[m,r)
            int m = binary_search([&](int i) -> bool { return tmp[i].first >= x; }, r, l - 1, 0);
            if (l < m) {
                if (!root->l) {
                    root->l = ST.new_node({tmp.begin() + l, tmp.begin() + m});
                } else {
                    dfs(dfs, root->l, l, m);
                }
                root->l->p = root;
            }
            if (m < r) {
                if (!root->r) {
                    root->r = ST.new_node({tmp.begin() + m, tmp.begin() + r});
                } else {
                    dfs(dfs, root->r, m, r);
                }
                root->r->p = root;
            }
            root->update();
        };
        dfs(dfs, f.root, 0, len(tmp));
        return f;
    }
    FUNC sum_all(vector<FUNC> &funcs) {
        assert(len(funcs) >= 1);
        T x0 = funcs[0].x0, x1 = funcs[0].x1;
        for (auto &g : funcs) chmax(x0, g.x0), chmin(x1, g.x1);
        if (x0 > x1) {
            for (auto &f : funcs) {
                ST.free_subtree(f.root);
            }
            return {nullptr, numeric_limits<T>::max() / 2, -numeric_limits<T>::max() / 2, 0, 0};
        }
        for (auto &f : funcs)
            f = restrict_domain(f, x0, x1);
        int idx = 0;
        For(i, 1, len(funcs)) if (len(funcs[idx]) < len(funcs[i])) idx = i;
        swap(funcs[idx], funcs.back());
        FUNC f = POP(funcs);
        vector<pair<T, T>> dat;
        for (auto &g : funcs) {
            f.y0 += g.y0, f.a0 += g.a0;
            auto tmp = ST.get_all(g.root);
            concat(dat, tmp);
            ST.free_subtree(g.root);
        }
        sort(dat.begin(), dat.end());
        // After that, simply insert dat
        if (!f.root) {
            f.root = ST.new_node(dat);
            return f;
        }
        auto dfs = [&](auto &dfs, np root, int l, int r) -> void {
            if (l == r)
                return;
            root->prop();
            T x = root->x.first;
            // [l,m),[m,r)
            int m = binary_search([&](int i) -> bool { return dat[i].first >= x; }, r, l - 1, 0);
            if (l < m) {
                if (!root->l) {
                    root->l = ST.new_node({dat.begin() + l, dat.begin() + m});
                } else {
                    dfs(dfs, root->l, l, m);
                }
                root->l->p = root;
            }
            if (m < r) {
                if (!root->r) {
                    root->r = ST.new_node({dat.begin() + m, dat.begin() + r});
                } else {
                    dfs(dfs, root->r, m, r);
                }
                root->r->p = root;
            }
            root->update();
        };
        dfs(dfs, f.root, 0, len(dat));
        return f;
    }

    FUNC shift(FUNC &f, T add_x, T add_y) {
        ST.apply(f.root, add_x);
        f.x0 += add_x, f.x1 += add_x, f.y0 += add_y;
        return f;
    }

    // h[z]=min(x+y==z)f(x)+g(y)
    FUNC convolve(FUNC &f, FUNC &g) {
        if (f.x0 > f.x1 || g.x0 > g.x1) {
            return {nullptr, numeric_limits<T>::max() / 2, -numeric_limits<T>::max() / 2, 0, 0};
        }
        if (len(f) < len(g)) swap(f, g);
        shift(f, g.x0, g.y0), shift(g, -g.x0, -g.y0);
        if (len(g) == 0) {
            return convolve_segment(f, 0, g.x1, g.a0, 0);
        }
        auto tmp = ST.get_all(g.root);
        ST.free_subtree(g.root);
        f = convolve_segment(f, 0, tmp[0].first, g.a0, 0);
        T a = g.a0;
        For(i, len(tmp)) {
            T nx = (i + 1 < len(tmp) ? tmp[i + 1].first : g.x1);
            a += tmp[i].second;
            f = convolve_segment(f, 0, nx - tmp[i].first, a, 0);
            for (auto &[x, a] : ST.get_all(f.root)) {
                assert(f.x0 <= x && x <= f.x1);
                if (f.root)
                    assert(!f.root->p);
            }
        }
        return f;
    }

    // [x0,x1], y=ax+b
    FUNC convolve_segment(FUNC &f, T x0, T x1, T a, T b) {
        assert(x0 <= x1);
        if (f.x0 > f.x1) {
            return {nullptr, numeric_limits<T>::max() / 2, -numeric_limits<T>::max() / 2, 0, 0};
        }
        f = shift(f, x0, a * x0 + b);
        T d = x1 - x0;
        if (d == 0)
            return f;
        // Insert the line segment from (0, 0) to (x1, ax1) somewhere.
        // In particular, x0 and y0 can remain as they are.
        if (f.x0 == f.x1) {
            return {nullptr, f.x0, f.x0 + d, a, f.y0};
        }
        // When it can be inserted at the beginning
        if (a <= f.a0) {
            ST.apply(f.root, d);
            f.root = ST.merge(ST.new_node({f.x0 + d, f.a0 - a}), f.root);
            f.x1 += d, f.a0 = a;
            return f;
        }
        // When it can be inserted at the end
        T a_last = f.a0 + ST.prod(f.root).first;
        if (a_last <= a) {
            f.root = ST.merge(f.root, ST.new_node({f.x1, a - a_last}));
            f.x1 += d;
            return f;
        }
        // Insert somewhere in between
        auto [l, r] = ST.split_max_right_prod(f.root, [&](auto prod) -> bool {
            return f.a0 + prod.first < a;
        });
        T asum = ST.prod(l).first;
        T a1 = a - (asum + f.a0);
        auto [xx, aa] = ST.get(r, 0);
        ST.apply(r, d);
        ST.set(r, 0, {xx + d, aa - a1});
        f.root = ST.merge3(l, ST.new_node({xx, a1}), r);
        f.x1 += d;
        return f;
    }

    FUNC add_const(FUNC &f, T a) {
        f.y0 += a;
        return f;
    }

    FUNC add_linear(FUNC &f, T a, T b) {
        f.y0 += a * f.x0 + b;
        f.a0 += a;
        return f;
    }

    // (a-x)+
    FUNC add_a_minus_x(FUNC &f, T a) {
        auto [x0, x1] = domain(f);
        if (x0 > x1) return f;
        if (a <= x0) return f;
        if (x1 <= a) return add_linear(f, -1, a);
        vector<pair<T, T>> point;
        point.emplace_back(x0, a - x0), point.emplace_back(a, 0), point.emplace_back(x1, 0);
        FUNC g = from_points(point);
        return add(f, g);
    }

    // (x-a)+
    FUNC add_x_minus_a(FUNC &f, T a) {
        auto [x0, x1] = domain(f);
        if (x0 > x1) return f;
        if (a <= x0) return add_linear(f, 1, -a);
        if (x1 <= a) return f;
        vector<pair<T, T>> point;
        point.emplace_back(x0, 0), point.emplace_back(a, 0), point.emplace_back(x1, x1 - a);
        FUNC g = from_points(point);
        return add(f, g);
    }

    // |x-a|
    FUNC add_abs(FUNC &f, T a) {
        f = add_a_minus_x(f, a);
        f = add_x_minus_a(f, a);
        return f;
    }

    // fx,x
    pair<T, T> get_min(FUNC &f) {
        if (f.x0 > f.x1)
            return {numeric_limits<T>::max() / 2, 0};
        if (f.a0 >= 0) {
            return {f.y0, f.x0};
        }
        auto [l, r] = ST.split_max_right_prod(f.root, [&](auto prod) -> bool {
            return f.a0 + prod.first < 0;
        });
        auto [asum, xasum] = ST.prod(l);
        T x = (r ? ST.get(r, 0).first : f.x1);
        f.root = ST.merge(l, r);
        T y = f.y0 + f.a0 * (x - f.x0) + x * asum - xasum;
        return {y, x};
    }

    FUNC clear_right(FUNC &f) {
        if (f.a0 >= 0) {
            ST.free_subtree(f.root), f.root = nullptr, f.a0 = 0;
            return f;
        }
        auto [l, r] = ST.split_max_right_prod(f.root, [&](auto prod) -> bool {
            return f.a0 + prod.first < 0;
        });
        f.root = l;
        if (!r) { return f; }
        T x = ST.get(r, 0).first;
        ST.free_subtree(r);
        f.root = ST.merge(f.root, ST.new_node({x, -(f.a0 + ST.prod(l).first)}));
        return f;
    }
    FUNC clear_left(FUNC &f) {
        if (f.a0 >= 0) {
            return f;
        }
        auto [l, r] = ST.split_max_right_prod(f.root, [&](auto prod) -> bool {
            return f.a0 + prod.first < 0;
        });
        auto [asum, xasum] = ST.prod(l);
        if (!r) {
            // Make it a constant
            T x = f.x1;
            T y = f.y0 + f.a0 * (x - f.x0) + x * asum - xasum;
            ST.free_subtree(l);
            f.root = nullptr;
            f.y0 = y, f.a0 = 0;
            return f;
        }
        T x = ST.get(f.root, 0).first;
        T y = f.y0 + f.a0 * (x - f.x0) + x * asum - xasum;
        T a = f.a0 + asum + ST.get(r, 0).second;
        ST.free_subtree(l);
        f.root = r;
        ST.set(r, 0, {x, a});
        f.y0 = y;
        f.a0 = 0;
        return f;
    }

    // void debug(FUNC &f) {
    //     auto dat = ST.get_all(f.root);
    //     debug(f.x0, f.x1, f.a0, f.y0);
    //     debug(dat);
    // }
private:
    template <typename F>
    long long binary_search(F check, long long ok, long long ng, bool check_ok = true) {
        if (check_ok) assert(check(ok));
        while (abs(ok - ng) > 1) {
            auto x = (ng + ok) / 2;
            (check(x) ? ok : ng) = x;
        }
        return ok;
    }
};
} // namespace SLOPE_TRICK_SUPER
using SLOPE_TRICK_SUPER::Slope_Trick_Super;
} // namespace mitsuha
#endif // AJAY_SLOPE_TRICK_SUPER
