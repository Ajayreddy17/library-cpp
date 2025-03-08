#ifndef AJAY_SLIDING_WINDOW_AGGREGATION
#define AJAY_SLIDING_WINDOW_AGGREGATION

namespace mitsuha{
template <class Monoid>
struct Sliding_Window_Aggregation {
    using X = typename Monoid::value_type;
    using value_type = X;
    int sz = 0;
    vector<X> dat;
    vector<X> cum_l;
    X cum_r;

    Sliding_Window_Aggregation()
            : cum_l({Monoid::unit()}), cum_r(Monoid::unit()) {}

    int size() { return sz; }

    void push(X x) {
        ++sz;
        cum_r = Monoid::op(cum_r, x);
        dat.emplace_back(x);
    }

    void pop() {
        --sz;
        cum_l.pop_back();
        if (len(cum_l) == 0) {
            cum_l = {Monoid::unit()};
            cum_r = Monoid::unit();
            while (len(dat) > 1) {
                cum_l.emplace_back(Monoid::op(dat.back(), cum_l.back()));
                dat.pop_back();
            }
            dat.pop_back();
        }
    }

    X lprod() { return cum_l.back(); }
    X rprod() { return cum_r; }

    X prod() { return Monoid::op(cum_l.back(), cum_r); }
};
 
// Constant multiplication will noticeably slow down the process, so don't use it when a queue is fine.
template <class Monoid>
struct SWAG_deque {
    using X = typename Monoid::value_type;
    using value_type = X;
    int sz;
    vector<X> dat_l, dat_r;
    vector<X> cum_l, cum_r;

    SWAG_deque() : sz(0), cum_l({Monoid::unit()}), cum_r({Monoid::unit()}) {}

    int size() { return sz; }

    void push_back(X x) {
        ++sz;
        dat_r.emplace_back(x);
        cum_r.emplace_back(Monoid::op(cum_r.back(), x));
    }

    void push_front(X x) {
        ++sz;
        dat_l.emplace_back(x);
        cum_l.emplace_back(Monoid::op(x, cum_l.back()));
    }

    void push(X x) { push_back(x); }

    void clear() {
        sz = 0;
        dat_l.clear(), dat_r.clear();
        cum_l = {Monoid::unit()}, cum_r = {Monoid::unit()};
    }

    void pop_front() {
        if (sz == 1) return clear();
        if (dat_l.empty()) rebuild();
        --sz;
        dat_l.pop_back();
        cum_l.pop_back();
    }

    void pop_back() {
        if (sz == 1) return clear();
        if (dat_r.empty()) rebuild();
        --sz;
        dat_r.pop_back();
        cum_r.pop_back();
    }

    void pop() { pop_front(); }

    X lprod() { return cum_l.back(); }
    X rprod() { return cum_r.back(); }
    X prod() { return Monoid::op(cum_l.back(), cum_r.back()); }
    X prod_all() { return prod(); }

private:
    void rebuild() {
        vector<X> X;
        Frr(i, len(dat_l)) X.emplace_back(dat_l[i]);
        X.insert(X.end(), dat_r.begin(). dat_r.end());
        clear();
        int m = len(X) / 2;
        Frr(i, m) push_front(X[i]);
        For(i, m, len(X)) push_back(X[i]);
        assert(sz == len(X));
    }
};
} // namespace mitsuha
#endif // AJAY_SLIDING_WINDOW_AGGREGATION