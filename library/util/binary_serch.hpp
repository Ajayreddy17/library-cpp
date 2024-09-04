#ifndef AJAY_BINARY_SEARCH
#define AJAY_BINARY_SEARCH

namespace mitsuha{
enum binary_search_tag { FirstFalse, FirstTrue, LastFalse, LastTrue };
 
template <binary_search_tag tag, typename Arg, typename Fun>
Arg binary_search_fisrt(Arg l, Arg r, const Fun &f) {
    const bool invert = tag == binary_search_tag::FirstFalse;
    Assert(invert ^ f(r));
    while (l < r) {
        Arg m = fld<Arg>(l + r, 2);
        (invert ^ f(m)) ?  r = m: l = m + 1;
    }
    return l;
}
 
template <binary_search_tag tag, typename Arg, typename Fun>
Arg binary_search_last(Arg l, Arg r, const Fun &f) {
    const bool invert = tag == binary_search_tag::LastFalse;
    Assert(invert ^ f(l));
    while (l < r) {
        Arg m = cld<Arg>(l + r, 2);
        (invert ^ f(m)) ?  l = m: r = m - 1;
    }
    return l;
}
 
template <binary_search_tag tag, typename Arg, 
        typename Fun, std::enable_if_t<std::conjunction_v<std::is_invocable<Fun, Arg>, std::is_integral<Arg>>, std::nullptr_t> = nullptr>
Arg binary_search(Arg l, Arg r, const Fun &f) {
    if constexpr (tag == binary_search_tag::FirstTrue or tag == binary_search_tag::FirstFalse)
        return binary_search_fisrt<tag>(l, r, f);
    else
        return binary_search_last<tag>(l, r, f);
}
} // namespace mitsuha;
#endif // AJAY_BINARY_SEARCH