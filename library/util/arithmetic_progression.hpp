#ifndef AJAY_ARITHMETIC_PROGRESSION
#define AJAY_ARITHMETIC_PROGRESSION

namespace mitsuha{
template<typename T>
struct Arithmetic_Progression{
    T diff;
    Arithmetic_Progression() {}
    Arithmetic_Progression(T diff): diff(diff) {}

    void set_diff(T dif){
        diff = dif;
    }

    // gives sum of {st, st + diff, .... st + (k - 1) * diff} i.e k terms
    T k_forward_sum(T st, T k){
        Assert(k >= T(0));
        return k * (2 * st + (k - 1) * diff) / T(2);
    }
    // gives sum of {st, st - diff, .... st - (k - 1) * diff} i.e k terms
    T k_backward_sum(T st, T k){
        Assert(k >= T(0));
        return k * (2 * st + (1 - k) * diff) / T(2);
    }
    
    // sum of {st, st + diff ... st + (k - 1) * diff < ed}
    // Dont forget to set appropriate diff before using
    T range_sum(T st, T ed){
        Assert(diff != T(0));
        if (st == ed) return T(0);
        if (diff > 0) {
            Assert(ed > st);
            T D = ed - st - 1;
            T k = fld(D, diff);
            return k_forward_sum(st, k + 1);
        }
        if (diff < 0) {
            Assert(st > ed);
            T D = st - ed - 1;
            T k = fld(D, -diff);
            return k_forward_sum(st, k + 1);
        }
        return T{};
    }

    T range_count(T st, T ed){
        Assert(diff != T(0));
        if (st == ed) return T(0);
        if (diff > 0) {
            Assert(ed > st);
            T D = ed - st - 1;
            return 1 + fld(D, diff);
        }
        if (diff < 0) {
            Assert(st > ed);
            T D = st - ed - 1;
            return 1 + fld(D, -diff);
        }
        return T{};
    }
};
} // namespace mitsuha
#endif // AJAY_ARITHMETIC_PROGRESSION