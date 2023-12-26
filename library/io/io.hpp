#ifndef AJAY_IO
#define AJAY_IO

namespace mitsuha::io {

    template <typename T>
    struct is_container {
        template <typename T2>
        static auto test(T2 t) -> decltype(++t.begin() != t.end(), *t.begin(), std::true_type{});
        static std::false_type test(...);
    public:
        static constexpr bool value = decltype(test(std::declval<T>()))::value;
    };
    template <typename T>
    constexpr bool is_container_v = is_container<T>::value;

    template <typename T>
    using is_integral = std::disjunction<std::is_integral<T>, std::is_same<T, __int128_t>, std::is_same<T, __uint128_t>>;
    template <typename T>
    constexpr bool is_integral_v = is_integral<T>::value;

    void read(char &c) { do c = getchar(); while (not isgraph(c)); }
    template <typename T, std::enable_if_t<is_integral_v<T>, std::nullptr_t> = nullptr>
    void read(T& x) {
        char c;
        do c = getchar(); while (not isgraph(c));
        if (c == '-') {
            read<T>(x), x = -x;
            return;
        }
        if (not (isdigit(c))) throw - 1;
        x = 0;
        do x = x * 10 + (std::exchange(c, getchar()) - '0'); while (isdigit(c));
    }
    template <typename T, std::enable_if_t<is_container_v<T>, std::nullptr_t> = nullptr>
    void read(T& x) { for (auto &e : x) read(e); }
    void read(std::string& x) {
        x.clear();
        char c;
        do c = getchar(); while (not isgraph(c));
        do x += std::exchange(c, getchar()); while (isgraph(c));
    }
        
    void write(char c) { putchar(c); }
    template <typename T, std::enable_if_t<is_integral_v<T>, std::nullptr_t> = nullptr>
    void write(T x) {
        static char buf[50];
        if constexpr (std::is_signed_v<T>) if (x < 0) putchar('-'), x = -x;
        int i = 0;
        do buf[i++] = '0' + (x % 10), x /= 10; while (x);
        while (i--) putchar(buf[i]);
    }
    template <typename T, std::enable_if_t<is_container_v<T>, std::nullptr_t> = nullptr>
    void write(const T& x) {
        bool insert_delim = false;
        for (auto it = x.begin(); it != x.end(); ++it) {
            if (std::exchange(insert_delim, true)) write(' ');
            write(*it);
        }
    }
    void write(const std::string& x) { for (char c : x) putchar(c); }
    
    template <typename ...Args>
    void read(Args &...args) { (read(args), ...); }
    template <typename Head, typename ...Tails>
    void print(Head&& head, Tails &&...tails) { write(head), ((write(' '), write(tails)), ...), write('\n'); }
}
namespace mitsuha{
    using io::print;
    using io::read;
    using io::write;
}
#endif // AJAY_IO