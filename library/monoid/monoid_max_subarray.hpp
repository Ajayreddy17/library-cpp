#ifndef AJAY_MONOID_MAX_SUBARRAY
#define AJAY_MONOID_MAX_SUBARRAY

namespace mitsuha{
template<typename T>
struct Monoid_Max_SubArray {
    struct X{ T ret = 0, sum = 0, pre = 0, suf = 0; };
    using value_type = X;
    static X op(const X &x, const X &y) { 
        X ret;
        ret.ret = max({x.pre + y.suf, x.ret, y.ret});
        ret.sum = x.sum + y.sum;
        ret.suf = max(x.suf, x.sum + y.suf);
        ret.pre = max(y.pre, y.sum + x.pre);
        return ret;
    }
    static constexpr X from_element(T x){ 
        return X{max(x, T(0)), x, max(x, T(0)), max(x, T(0))};
    }
    static constexpr X unit() { return X{}; }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MAX_SUBARRAY