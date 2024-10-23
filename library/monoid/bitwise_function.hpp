#ifndef AJAY_BITWISE_FUNCTION
#define AJAY_BITWISE_FUNCTION

namespace mitsuha{
// composition of bitwise operation
// 0 -> a, -1 -> b
template <typename UINT>
struct Bitwise_Function {
    static_assert(is_same_v<UINT, unsigned int> or is_same_v<UINT, unsigned long long>);
    using value_type = pair<UINT, UINT>;
    using X = value_type;
    static X op(X x, X y) { return {eval(y, x.first), eval(y, x.second)}; }
    static UINT eval(X f, UINT x) { return (f.first & (~x)) | (f.second & x); }
    static X func_or(UINT x) { return {x, UINT(-1)}; }
    static X func_and(UINT x) { return {0, x}; }
    static X func_xor(UINT x) { return {x, UINT(-1) ^ x}; }
    static constexpr X unit() { return {UINT(0), UINT(-1)}; }
    static constexpr bool commute = 0;
};
} // namespace mitsuha
#endif // AJAY_BITWISE_FUNCTION