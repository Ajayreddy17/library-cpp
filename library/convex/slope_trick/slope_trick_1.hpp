#ifndef AJAY_SLOPE_TRICK_1
#define AJAY_SLOPE_TRICK_1

#include "library/datastructure/double_end_queue_const_add.hpp"
#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
struct Slope_Trick_1 {
    struct FUNC {
        // Keep both ends of the domain in the queue to ensure the queue is never empty.
        Double_End_Queue_Const_Add<Monoid_Add<long long>> que_l, que_r;
        __int128 min_f = 0;
        int size() { return que_l.size() + que_r.size(); }
    };

    // O(|a|)
    FUNC segment_func(long long L, long long R, long long a, long long b) {
        FUNC f;
        if (a >= 0) {
            f.min_f = __int128(a) * L + b;
            f.que_l.push(L);
            For(a) f.que_r.push(L);
            f.que_r.push(R);
        } else {
            f.min_f = __int128(a) * R + b;
            f.que_r.push(R);
            For(-a) f.que_l.push(R);
            f.que_l.push(L);
        }
        return f;
    }

    pair<long long, long long> domain(FUNC &f) { return {f.que_l.min(), f.que_r.max()}; }

    // O(N)
    __int128 eval(FUNC &f, long long x) {
        auto [x0, x1] = domain(f);
        assert(x0 <= x && x <= x1);
        __int128 ans = f.min_f;
        f.que_l.enumerate_all([&](long long l) -> void { ans += max<long long>(l - x, 0); });
        f.que_r.enumerate_all([&](long long r) -> void { ans += max<long long>(x - r, 0); });
        return ans;
    }

    // implemented only for cases where the original min_f intersects with the domain.
    FUNC restrict_domain(FUNC &f, long long L, long long R) {
        auto [x0, x1] = domain(f);
        chmax(L, x0), chmin(R, x1);
        while (!f.que_l.empty() && f.que_l.min() <= L) { f.que_l.pop_min(); }
        while (!f.que_r.empty() && f.que_r.max() >= R) { f.que_r.pop_max(); }
        f.que_l.push(L);
        f.que_r.push(R);
        return f;
    }

    // +(ax+b), O(|a|*log)
    FUNC add_linear(FUNC &f, long long a, long long b) {
        auto [x0, x1] = domain(f);
        f.min_f += b;
        while (a > 0) {
            f.min_f += f.que_l.pop_max();
            f.que_r.push(f.que_l.pop_max());
            if (f.que_l.empty()) f.que_l.push(x0);
            --a;
        }
        while (a < 0) {
            f.min_f -= f.que_r.pop_min();
            f.que_l.push(f.que_r.pop_min());
            if (f.que_r.empty()) f.que_r.push(x0);
            ++a;
        }
        return f;
    }

    // (a-x)+
    FUNC add_a_minus_x(FUNC &f, long long a) {
        auto [x0, x1] = domain(f);
        assert(x0 <= x1);
        if (a <= x0) return f;
        if (x1 <= a) return add_linear(f, -1, a);
        long long x = f.que_r.min();
        f.min_f += max<long long>(a - x, 0);
        if (a <= x) { 
            f.que_l.push(a);
        } else {
            f.que_l.push(f.que_r.pop_min());
            f.que_r.push(a);
        }
        return f;
    }

    // (x-a)+
    FUNC add_x_minus_a(FUNC &f, long long a) {
        auto [x0, x1] = domain(f);
        assert(x0 <= x1);
        if (a <= x0) return add_linear(f, 1, -a);
        if (x1 <= a) return f;
        long long x = f.que_l.max();
        f.min_f += max<long long>(x - a, 0);
        if (a >= x) {
            f.que_r.push(a);
        } else {
            f.que_r.push(f.que_l.pop_max());
            f.que_l.push(a);
        }
        return f;
    }

    // (x-a)+
    FUNC add_abs(FUNC &f, long long a) {
        f = add_a_minus_x(f, a);
        f = add_x_minus_a(f, a);
        return f;
    }

    FUNC clear_inc(FUNC &f) {
        auto [x0, x1] = domain(f);
        f.que_r.clear();
        f.que_r.push(x1);
        return f;
    }

    FUNC add(FUNC &f, FUNC &g) {
        auto [a0, a1] = domain(f);
        auto [b0, b1] = domain(g);
        long long x0 = max(a0, b0);
        long long x1 = min(a1, b1);
        assert(x0 <= x1);
        restrict_domain(f, x0, x1), restrict_domain(g, x0, x1);
        if (len(f) < len(g)) swap(f, g);
        f.min_f += g.min_f;
        for (auto l : g.que_l.dat) {
            l += g.que_l.add;
            // (l-x)+
            if (l <= f.que_r.min()) {
                f.que_l.push(l);
            } else {
                f.que_l.push(f.que_r.pop_min());
                f.que_r.push(l);
            }
            long long x = f.que_r.min();
            f.min_f += max<long long>(0, l - x);
        }
        return f;
    }

    // FUNC sum_all(vc<FUNC> &funcs) {}

    // FUNC shift(FUNC &f, T add_x, T add_y) {
    //   ST.apply(f.root, add_x);
    //   f.x0 += add_x, f.x1 += add_x, f.y0 += add_y;
    //   return f;
    // }

    // h[z]=min(x+y==z)f(x)+g(y)
    // FUNC convolve(FUNC &f, FUNC &g) {
    //   if (f.x0 > f.x1 || g.x0 > g.x1) { return {nullptr, infty<T>, -infty<T>, 0, 0}; }
    //   if (len(f) < len(g)) swap(f, g);
    //   shift(f, g.x0, g.y0), shift(g, -g.x0, -g.y0);
    //   if (len(g) == 0) { return convolve_segment(f, 0, g.x1, g.a0, 0); }
    //   auto tmp = ST.get_all(g.root);
    //   ST.free_subtree(g.root);
    //   f = convolve_segment(f, 0, tmp[0].fi, g.a0, 0);
    //   T a = g.a0;
    //   FOR(i, len(tmp)) {
    //     T nx = (i + 1 < len(tmp) ? tmp[i + 1].fi : g.x1);
    //     a += tmp[i].se;
    //     f = convolve_segment(f, 0, nx - tmp[i].fi, a, 0);
    //     for (auto &[x, a]: ST.get_all(f.root)) {
    //       assert(f.x0 <= x && x <= f.x1);
    //       if (f.root) assert(!f.root->p);
    //     }
    //   }
    //   return f;
    // }

    // [x0,x1], y=ax+b
    // FUNC convolve_segment(FUNC &f, T x0, T x1, T a, T b) {
    //   assert(x0 <= x1);
    //   if (f.x0 > f.x1) { return {nullptr, infty<T>, -infty<T>, 0, 0}; }
    //   f = shift(f, x0, a * x0 + b);
    //   T d = x1 - x0;
    //   if (d == 0) return f;
    //   // (0,0) から (x1,ax1) への線分をどこかに挿入する
    //   // 特に x0, y0 はこのままでよい
    //   if (f.x0 == f.x1) { return {nullptr, f.x0, f.x0 + d, a, f.y0}; }
    //   // 先頭に挿入できる場合
    //   if (a <= f.a0) {
    //     ST.apply(f.root, d);
    //     f.root = ST.merge(ST.new_node({f.x0 + d, f.a0 - a}), f.root);
    //     f.x1 += d, f.a0 = a;
    //     return f;
    //   }
    //   // 末尾に挿入できる場合
    //   T a_last = f.a0 + ST.prod(f.root).fi;
    //   if (a_last <= a) {
    //     f.root = ST.merge(f.root, ST.new_node({f.x1, a - a_last}));
    //     f.x1 += d;
    //     return f;
    //   }
    //   // 間のどこかに挿入
    //   auto [l, r] = ST.split_max_right_prod(f.root, [&](auto prod) -> bool { return
    //   f.a0 + prod.fi < a; }); T asum = ST.prod(l).fi; T a1 = a - (asum + f.a0); auto
    //   [xx, aa] = ST.get(r, 0); ST.apply(r, d); ST.set(r, 0, {xx + d, aa - a1}); f.root
    //   = ST.merge3(l, ST.new_node({xx, a1}), r); f.x1 += d; return f;
    // }

    // fx, xl, xr
    tuple<__int128, long long, long long> get_min(FUNC &f) {
        return {f.min_f, f.que_l.max(), f.que_r.min()};
    }
};
} // namespace mitsuha
#endif // AJAY_SLOPE_TRICK_1