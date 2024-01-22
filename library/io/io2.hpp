#ifndef AJAY_IO2
#define AJAY_IO2

#include <unistd.h>
namespace mitsuha::io {
    static constexpr uint32_t SZ = 1 << 17;
    char ibuf[SZ], obuf[SZ], out[100];
    // pointer of ibuf, obuf
    uint32_t pil = 0, pir = 0, por = 0;

    template <typename T>
    struct has_read_method {
        template <typename U> static std::true_type test(decltype(&U::read) *);
        template <typename> static std::false_type test(...);
        using type = decltype(test<T>(nullptr));
        static constexpr bool value = type::value;
    };
    template <typename T>
    struct has_print_method {
        template <typename U> static std::true_type test(decltype(&U::print) *);
        template <typename> static std::false_type test(...);
        using type = decltype(test<T>(nullptr));
        static constexpr bool value = type::value;
    };
    template <typename T> typename enable_if<has_read_method<T>::value, void>::type rd(T &x) { x.read(); }
    template <typename T> typename enable_if<has_print_method<T>::value, void>::type wt(T x) { x.print(); }

    struct Pre {
        char num[10000][4];
        constexpr Pre() : num() {
            for (int i = 0; i < 10000; i++) {
                int n = i;
                for (int j = 3; j >= 0; j--) {
                    num[i][j] = n % 10 | '0';
                    n /= 10;
                }
            }
        }
    } constexpr pre;

    inline void load() {
        memcpy(ibuf, ibuf + pil, pir - pil);
        pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
        pil = 0;
    }
    inline void flush() { fwrite(obuf, 1, por, stdout); por = 0; }

    void rd(char &c) { do { if (pil + 1 > pir) load(); c = ibuf[pil++]; } while (isspace(c)); }
    void rd(string &x) {
        x.clear();
        char c;
        do { if (pil + 1 > pir) load(); c = ibuf[pil++]; } while (isspace(c));
        do { x += c; if (pil == pir) load(); if (pil == pir) break; c = ibuf[pil++]; } while (!isspace(c));
    }

    template <typename T> void rd_real(T &x) { string s; rd(s); x = stod(s); }

    template <typename T>
    void rd_integer(T &x) {
        if (pil + 100 > pir) load();
        char c;
        do { c = ibuf[pil++]; }while (c < '-');
        bool minus = 0;
        if constexpr (is_signed<T>::value || is_same_v<T, __int128>) { if (c == '-') { minus = 1, c = ibuf[pil++]; } }
        x = 0;
        while (c >= '0') { x = x * 10 + (c & 15), c = ibuf[pil++]; }
        if constexpr (is_signed<T>::value || is_same_v<T, __int128>) { if (minus) x = -x; }
    }

    void rd(int &x) { rd_integer(x); }
    void rd(long long &x) { rd_integer(x); }
    void rd(__int128 &x) { rd_integer(x); }
    void rd(unsigned int &x) { rd_integer(x); }
    void rd(unsigned long long &x) { rd_integer(x); }
    void rd(unsigned __int128 &x) { rd_integer(x); }
    void rd(double &x) { rd_real(x); }
    void rd(long double &x) { rd_real(x); }
    void rd(__float128 &x) { rd_real(x); }

    template <class T, class U> void rd(pair<T, U> &p) { return rd(p.first), rd(p.second); }
    template <size_t N = 0, typename T>
    void rd(T &t) { if constexpr (N < std::tuple_size<T>::value) { auto &x = std::get<N>(t); rd(x); rd<N + 1>(t); } }
    template <class... T> void rd(tuple<T...> &tpl) { rd(tpl); }
    template <size_t N = 0, typename T> void rd(array<T, N> &x) { for (auto &d: x) rd(d); }
    template <class T> void rd(vector<T> &x) { for (auto &d: x) rd(d); }

    void read() {}
    template <class H, class... T> void read(H &h, T &... t) { rd(h), read(t...); }

    void wt(const char c) { if (por == SZ) flush(); obuf[por++] = c; }
    void wt(const string &s) { for (char c: s) wt(c); }
    void wt(const char *s) { size_t len = strlen(s); for (size_t i = 0; i < len; i++) wt(s[i]); }

    template <typename T>
    void wt_integer(T x) {
        if (por > SZ - 100) flush();
        if (x < 0) { obuf[por++] = '-', x = -x; }
        int outi;
        for (outi = 96; x >= 10000; outi -= 4) {
            memcpy(out + outi, pre.num[x % 10000], 4);
            x /= 10000;
        }
        if (x >= 1000) {
            memcpy(obuf + por, pre.num[x], 4);
            por += 4;
        } else if (x >= 100) {
            memcpy(obuf + por, pre.num[x] + 1, 3);
            por += 3;
        } else if (x >= 10) {
            int q = (x * 103) >> 10;
            obuf[por] = q | '0';
            obuf[por + 1] = (x - q * 10) | '0';
            por += 2;
        } else
            obuf[por++] = x | '0';
        memcpy(obuf + por, out + outi + 4, 96 - outi);
        por += 96 - outi;
    }

    template <typename T>
    void wt_real(T x) {
        ostringstream oss;
        oss << fixed << setprecision(15) << double(x);
        string s = oss.str();
        wt(s);
    }

    void wt(int x) { wt_integer(x); }
    void wt(long long x) { wt_integer(x); }
    void wt(__int128 x) { wt_integer(x); }
    void wt(unsigned int x) { wt_integer(x); }
    void wt(unsigned long long x) { wt_integer(x); }
    void wt(unsigned __int128 x) { wt_integer(x); }
    void wt(double x) { wt_real(x); }
    void wt(long double x) { wt_real(x); }
    void wt(__float128 x) { wt_real(x); }

    template <class T, class U>
    void wt(const pair<T, U> val) { wt(val.first); wt(' '); wt(val.second); }
    template <size_t N = 0, typename T> void wt_tuple(const T t) {
        if constexpr (N < std::tuple_size<T>::value) {
            if constexpr (N > 0) { wt(' '); }
            const auto x = std::get<N>(t);
            wt(x); wt_tuple<N + 1>(t);
        }
    }
    template <class... T> void wt(tuple<T...> tpl) { wt_tuple(tpl); }
    template <class T, size_t S> void wt(const array<T, S> val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) { if (i) wt(' '); wt(val[i]); }
    }
    template <class T> void wt(const vector<T> val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) { if (i) wt(' '); wt(val[i]); }
    }

    void print() { wt('\n'); }
    template <class Head, class... Tail>
    void print(Head &&head, Tail &&... tail) { wt(head); if (sizeof...(Tail)) wt(' '); print(forward<Tail>(tail)...); }
    void __attribute__((destructor)) _d() { flush(); }
} // namespace mitsuha::io
namespace mitsuha{ using io::read; using io::print; using io::flush; }
#endif // AJAY_IO2