#ifndef AJAY_SLOPE_TRICK_0
#define AJAY_SLOPE_TRICK_0

namespace mitsuha{
struct Slope_Trick_0 {
    static constexpr long long LMIN = -linf;
    static constexpr long long RMAX =  linf;
    max_priority_queue<long long> que_l;
    min_priority_queue<long long> que_r;

    long long add_l, add_r;
    int128 min_f; 

    Slope_Trick_0() : add_l(0), add_r(0), min_f(0) {}

    int size() { return len(que_l) + len(que_r); }
    // lx, rx, min_f
    tuple<long long, long long, int128> get_min() { return {top_L(), top_R(), min_f}; }

    // f(x) += a
    void add_const(long long a) { min_f += a; }

    // Add ax + b, O(|a| log N)
    void add_linear(long long a, long long b) {
        min_f += b;
        For(max<int>(a, 0)) {
            long long x = pop_L();
            min_f += x;
            push_R(x);
        }
        For(max<int>(-a, 0)) {
            long long x = pop_R();
            min_f -= x;
            push_L(x);
        }
    }

    // add \_
    // f(x) += max(a - x, 0)
    void add_left(long long a) {
        min_f += max<long long>(0, a - top_R());
        push_R(a), push_L(pop_R());
    }
    // add _/
    // f(x) += max(x - a, 0)
    void add_right(long long a) {
        min_f += max<long long>(0, top_L() - a);
        push_L(a), push_R(pop_L());
    }

    // add \/
    // f(x) += abs(x - a)
    void add_abs(long long a) { add_left(a), add_right(a); }

    // \/ -> \_
    // f_{new} (x) = min f(y) (y <= x)
    void clear_right() { que_r = min_priority_queue<long long>(); }
    // \/ -> _/
    // f_{new} (x) = min f(y) (y >= x)
    void clear_left() { que_l = max_priority_queue<long long>(); }

    void shift(const long long &a) { add_l += a, add_r += a; }

    // g(x) = min_{x-b <= y <= x-a} f(y)
    void sliding_window_minimum(const long long &a, const long long &b) { add_l += a, add_r += b; }

    // O(size log(size))
    int128 eval(long long x) {
        int128 y = min_f;
        auto que_l_copy = que_l;
        auto que_r_copy = que_r;
        while (len(que_l_copy)) {
            y += max<long long>(0, (que_l_copy.top() + add_l) - x);
            que_l_copy.pop();
        }
        while (len(que_r_copy)) {
            y += max<long long>(0, x - (que_r_copy.top() + add_r));
            que_r_copy.pop();
        }
        return y;
    }

private:
    void push_R(const long long &x) { que_r.emplace(x - add_r); }
    void push_L(const long long &x) { que_l.emplace(x - add_l); }
    long long top_R() {
        if (que_r.empty()) que_r.emplace(RMAX);
        return que_r.top() + add_r;
    }
    long long top_L() {
        if (que_l.empty()) que_l.emplace(LMIN);
        return que_l.top() + add_l;
    }
    long long pop_R() {
        long long res = top_R();
        que_r.pop();
        return res;
    }
    long long pop_L() {
        long long res = top_L();
        que_l.pop();
        return res;
    }
};

#ifdef LOCAL
ostream &operator<<(ostream &out, const Slope_Trick_0 &S){
    auto ST = S;
    vector<long long> left, right;
    while (len(ST.que_l)) {
        left.emplace_back(ST.que_l.top() + ST.add_l);
        ST.que_l.pop();
    }
    while (len(ST.que_r)) {
        right.emplace_back(ST.que_r.top() + ST.add_r);
        ST.que_r.pop();
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    out << "min_f " << ST.min_f << endl << "left ";
    for (auto &x: left) out << x << " ";
    out << endl << "right: ";
    for (auto &x: right) out << x << " ";
    return out;
}
#endif
} // namespace mitsuha
#endif // AJAY_SLOPE_TRICK_0