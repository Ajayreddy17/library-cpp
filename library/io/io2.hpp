#ifndef AJAY_IO2
#define AJAY_IO2

#include <unistd.h>

#define FASTIO
namespace mitsuha::io {
#define READ_INTEGRAL(type_t) void rd(type_t &x) { rd_integer(x); }
#define READ_FLOATING(type_t) void rd(type_t &x) { rd_real(x); }
#define WRITE_INTEGRAL(type_t) void wt(type_t x) { wt_integer(x); }
#define WRITE_FLOATING(type_t) void wt(type_t x) { wt_real(x); }

static constexpr uint32_t SZ = 1 << 17;
char input_buffer[SZ];
char output_buffer[SZ];
char out[100];
uint32_t pil = 0, pir = 0, por = 0;

struct Pre {
    char num[10000][4];
    constexpr Pre() : num() {
        for (int i = 0; i < 10000; i++) {
            for (int j = 3, n = i; j >= 0; j--, n /= 10) {
                num[i][j] = n % 10 | '0';
            }
        }
    }
} constexpr pre;

inline void load() {
    memcpy(input_buffer, input_buffer + pil, pir - pil);
    pir = pir - pil + fread(input_buffer + pir - pil, 1, SZ - pir + pil, stdin);
    pil = 0;
    if (pir < SZ) input_buffer[pir++] = '\n';
}
inline void flush() {
    fwrite(output_buffer, 1, por, stdout);
    por = 0;
}
void rd(char &c) { 
    do { 
        if (pil >= pir) load(); 
        c = input_buffer[pil++]; 
    } while (isspace(c));
}
void rd(std::string &x) {
    x.clear();
    char c;
    do { 
        if (pil >= pir) load(); 
        c = input_buffer[pil++]; 
    } while (isspace(c));
    do {
        x += c;
        if (pil == pir) load();
        c = input_buffer[pil++];
    } while (!isspace(c));
}
template<typename T>
void rd_real(T &x) {
    std::string s;
    rd(s);
    x = stod(s);
}
template<typename T>
void rd_integer(T &x) {
    if (pil + 100 > pir) load();
    char c;
    do c = input_buffer[pil++]; while (c < '-');
    bool minus = 0;
    if constexpr (std::is_signed<T>::value or std::is_same_v <T, __int128 >) {
        if (c == '-') {
            minus = 1;
            c = input_buffer[pil++];
        }
    }
    x = 0;
    while ('0' <= c) { x = x * 10 + (c & 15), c = input_buffer[pil++]; }
    if constexpr (std::is_signed<T>::value or std::is_same_v < T, __int128 >) {
        if (minus) x = -x;
    }
}

READ_INTEGRAL(int) 
READ_INTEGRAL(long int)
READ_INTEGRAL(long long)
READ_INTEGRAL(__int128)
READ_INTEGRAL(unsigned int)
READ_INTEGRAL(unsigned long long)
READ_INTEGRAL(unsigned __int128)
READ_FLOATING(double)
READ_FLOATING(long double)
READ_FLOATING(__float128)

template<class T, class U> void rd(std::pair <T, U> &p) {
    rd(p.first);
    rd(p.second);
}
template<size_t N = 0, typename T> void rd_tuple(T &t) {
    if constexpr (N < std::tuple_size<T>::value) {
        auto &x = std::get<N>(t);
        rd(x);
        rd_tuple<N + 1>(t);
    }
}
template<class... T> void rd(std::tuple<T...> &tpl) {
    rd_tuple(tpl);
}
template<size_t N = 0, typename T> void rd(std::array <T, N> &x) {
    for (auto &d: x) rd(d);
}
template<class T> void rd(std::vector <T> &x) {
    for (auto &d: x) rd(d);
}

void read() {}
template<class Head, class... Args>
void read(Head &h, Args &... t) {
    rd(h);
    read(t...);
}

void wt(const char c) {
    if (por == SZ) flush();
    output_buffer[por++] = c;
}
void wt(const std::string &s) {
    for (char c: s) wt(c);
}
void wt(const char *s) {
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) wt(s[i]);
}
template<typename T>
void wt_integer(T x) {
    if (por > SZ - 100) flush();
    if (x < 0) { output_buffer[por++] = '-', x = -x; }
    int outi;
    for (outi = 96; x >= 10000; outi -= 4, x /= 10000) {
        memcpy(out + outi, pre.num[x % 10000], 4);
    }
    if (x >= 1000) {
        memcpy(output_buffer + por, pre.num[x], 4);
        por += 4;
    }
    else if (x >= 100) {
        memcpy(output_buffer + por, pre.num[x] + 1, 3);
        por += 3;
    }
    else if (x >= 10) {
        int q = (x * 103) >> 10;
        output_buffer[por] = q | '0';
        output_buffer[por + 1] = (x - q * 10) | '0';
        por += 2;
    }
    else output_buffer[por++] = x | '0';
    memcpy(output_buffer + por, out + outi + 4, 96 - outi);
    por += 96 - outi;
}
template<typename T>
void wt_real(T x) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(15) << double(x);
    std::string s = oss.str();
    wt(s);
}

WRITE_INTEGRAL(int)
WRITE_INTEGRAL(long int)
WRITE_INTEGRAL(long long)
WRITE_INTEGRAL(__int128)
WRITE_INTEGRAL(unsigned int)
WRITE_INTEGRAL(unsigned long long)
WRITE_INTEGRAL(unsigned __int128)
WRITE_FLOATING(double)
WRITE_FLOATING(long double)
WRITE_FLOATING(__float128)

template<class T, class U>
void wt(const std::pair <T, U> val) {
    wt(val.first);
    wt(' ');
    wt(val.second);
}
template<size_t N = 0, typename T>
void wt_tuple(const T t) {
    if constexpr (N < std::tuple_size<T>::value) {
        if constexpr (N > 0) { wt(' '); }
        const auto x = std::get<N>(t);
        wt(x);
        wt_tuple<N + 1>(t);
    }
}
template<class... T> void wt(std::tuple<T...> tpl) {
    wt_tuple(tpl);
}
template<class T, size_t S> void wt(const std::array <T, S> val) {
    for (size_t i = 0, n = val.size(); i < n; i++) {
        if (i) wt(' ');
        wt(val[i]);
    }
}
template<class T> void wt(const std::vector<T> val) {
    for (size_t i = 0, n = val.size(); i < n; i++) {
        if (i) wt(' ');
        wt(val[i]);
    }
}

void print() { wt('\n'); }
template<class Head, class... Args>
void print(Head &&head, Args &&... args) {
    wt(head);
    if (sizeof...(Args)) wt(' ');
    print(std::forward<Args>(args)...);
}

void __attribute__((destructor)) _d() { flush(); }
} // namespace mitsuha::io

namespace mitsuha {
    using io::read; using io::print; using io::flush;
}
#endif // AJAY_IO2