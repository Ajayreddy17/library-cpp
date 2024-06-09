#ifndef AJAY_MONOID_ADD_ARRAY
#define AJAY_MONOID_ADD_ARRAY

namespace mitsuha{
template <typename E, int K>
struct Monoid_Add_Array {
    using value_type = array<E, K>;
    using X = value_type;
    static X op(X x, X y) {
        for(int i = 0; i < K; ++i) x[i] += y[i];
        return x;
    }
    static constexpr X unit() { return X{}; }
    static constexpr X inverse(X x) {
        for (auto& v: x) v = -v;
        return x;
    }
    static constexpr X power(X x, long long n) {
        for (auto& v: x) v *= E(n);
        return x;
    }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ADD_ARRAY
