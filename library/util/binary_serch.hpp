#ifndef AJAY_BINARY_SEARCH
#define AJAY_BINARY_SEARCH

namespace mitsuha{
enum binary_search_tag { FirstFalse, FirstTrue, LastFalse, LastTrue };

template <binary_search_tag tag, typename Arg, typename Fun,
        std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_integral<Arg>>, std::nullptr_t> = nullptr>
Arg binary_search_key_fisrt(Arg l, Arg r, const Fun &f) {
    const bool invert = tag == binary_search_tag::FirstFalse;
    assert(invert ^ f(r));
    while (l < r) {
        Arg m = midpoint(l, r);
        (invert ^ f(m)) ?  r = m: l = m + 1;
    }
    return l;
}

template <binary_search_tag tag, typename Arg, typename Fun,
        std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_integral<Arg>>, std::nullptr_t> = nullptr>
Arg binary_search_key_last(Arg l, Arg r, const Fun &f) {
    const bool invert = tag == binary_search_tag::LastFalse;
    assert(invert ^ f(l));
    while (l < r) {
        Arg m = midpoint(l + 1, r);
        (invert ^ f(m)) ?  l = m: r = m - 1;
    }
    return l;
}

template <binary_search_tag tag, typename Arg, typename Fun>
Arg binary_search_key(Arg l, Arg r, const Fun &f) {
    if constexpr (tag == binary_search_tag::FirstTrue or tag == binary_search_tag::FirstFalse)
        return binary_search_key_fisrt<tag>(l, r, f);
    else
        return binary_search_key_last<tag>(l, r, f);
}

template <binary_search_tag tag, typename Arg, typename Fun,
        std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_integral<Arg>>, std::nullptr_t> = nullptr>
std::invoke_result_t<Fun, Arg> binary_search_value(Arg l, Arg r, const Fun &f) {
    return f(binary_search_key<tag>(l, r, f));
}
} // namespace mitsuha;
#endif // AJAY_BINARY_SEARCH