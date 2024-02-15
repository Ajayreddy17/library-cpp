#ifndef AJAY_SEARCH
#define AJAY_SEARCH

namespace mitsuha{
enum class ternary_search_tag { Convex, Concave };

// argmin_{l<=x<=r} f(x) if f is convex, argmax_{l<=x<=r} f(x) if f is concave
template <ternary_search_tag tag, typename Arg, typename Fun, std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_floating_point<Arg>>, std::nullptr_t> = nullptr>
Arg ternary_search_key(Arg l, Arg r, std::size_t num, const Fun &f) {
    assert(l <= r);
    const auto compare = std::conditional_t<tag == ternary_search_tag::Convex, std::greater<>, std::less<>>{};
    while (num--) {
        const Arg ml = l + (r - l) / 3, mr = r - (r - l) / 3;
        if (compare(f(ml), f(mr))) l = ml;
        else r = mr;
    }
    return l;
}

// argmin_{l<=x<=r} f(x) if f is convex, argmax_{l<=x<=r} f(x) if f is concave
template <ternary_search_tag tag, typename Arg, typename Fun, std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_integral<Arg>>, std::nullptr_t> = nullptr>
Arg ternary_search_key(Arg l, Arg r, const Fun &f) {
    assert(l <= r);
    const auto compare = std::conditional_t<tag == ternary_search_tag::Convex, std::greater<>, std::less<>>{};
    while (r - l >= 3) {
        const Arg ml = l + (r - l) / 3, mr = r - (r - l) / 3;
        if (compare(f(ml), f(mr))) l = ml;
        else r = mr;
    }
    Arg x = l;
    auto fx = f(x);
    for (Arg y = l + 1; y <= r; ++y) {
        if (auto fy = f(y); compare(fx, fy)) x = y, fx = fy;
    }
    return x;
}

// min_{l<=x<=r} f(x) if f is convex, max_{l<=x<=r} f(x) if f is concave
template <ternary_search_tag tag, typename Arg, typename Fun, std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_floating_point<Arg>>, std::nullptr_t> = nullptr>
std::invoke_result_t<Fun, Arg> ternary_search_value(const Arg &l, const Arg &r, const std::size_t &num, const Fun &f) {
    return f(ternary_search_key<tag>(l, r, num, f));
}

// min_{l<=x<=r} f(x) if f is convex, max_{l<=x<=r} f(x) if f is concave
template <ternary_search_tag tag, typename Arg, typename Fun, std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_integral<Arg>>, std::nullptr_t> = nullptr>
std::invoke_result_t<Fun, Arg> ternary_search_value(Arg l, Arg r, const Fun &f) {
    return f(ternary_search_key<tag>(l, r, f));
}
} // namespace mitsuha
#endif // AJAY_SEARCH