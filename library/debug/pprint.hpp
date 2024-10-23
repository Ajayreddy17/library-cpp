#ifndef H_PRETTY_PRINT
#define H_PRETTY_PRINT

struct io_setup {
    io_setup(int precision = 15) {
#if defined(LOCAL) and not defined(STRESS)
        freopen("input.txt",  "r", stdin);
        freopen("output.txt", "w", stdout);
        freopen("error.txt", "w", stderr);
#endif
    }
} io_setup_{};

using namespace std;

namespace mitsuha{
template <typename Iterator>
string print_iterable(Iterator begin, Iterator end, string bef, string aft,
        function<string(typename iterator_traits<Iterator>::value_type)>* f = nullptr);

ostream &operator<<(ostream &out, __int128_t x);
ostream &operator<<(ostream &out, __uint128_t x);
 
template <typename S, typename T>
ostream& operator <<(ostream& out, const pair<S, T>& p);

template <typename ...Args>
ostream& operator <<(ostream& out, const tuple<Args...>& a);

template<class T>
ostream &operator<<(typename enable_if<!is_same<T, string>::value, ostream>::type &out, const T &arr);
 
#define _define_print_container(Container, bef, aft)        \
template <typename T>                                       \
ostream& operator <<(ostream& out, const Container<T>& v) { \
    out << print_iterable(v.begin(), v.end(), bef, aft);    \
    return out;                                             \
}                                                           \
 
_define_print_container(vector, "[", "]");
_define_print_container(set, "{", "}");
_define_print_container(unordered_set, "{", "}");
_define_print_container(multiset, "{", "}");
_define_print_container(unordered_multiset, "{", "}");
_define_print_container(deque, "[", "]");
 
#define _define_print_dictionary(Dictionary, bef, aft)                      \
template <typename T1, typename T2>                                         \
ostream& operator <<(ostream& out, const Dictionary<T1, T2>& v) {           \
    out << print_iterable(v.begin(), v.end(), bef, aft);                    \
    return out;                                                             \
}                                                                           \
 
_define_print_dictionary(map, "{", "}");
_define_print_dictionary(unordered_map, "{", "}");
 
#define _define_print_container_adaptor(Adaptor, OP)                        \
template <typename T>                                                       \
ostream& operator <<(ostream& out, Adaptor<T> a) {                          \
    vector<T> v;                                                            \
    while (!a.empty()) v.push_back(a.OP()), a.pop();                        \
    out << v;                                                               \
    return out;                                                             \
}                                                                           \
 
_define_print_container_adaptor(stack, top)
_define_print_container_adaptor(queue, front)
_define_print_container_adaptor(priority_queue, top)
 
template <typename Iterator>
string print_iterable(Iterator begin, Iterator end, string bef, string aft,
        function<string(typename iterator_traits<Iterator>::value_type)>* f) {
    stringstream res;
    res << bef;
    bool is_first = true;
    for (Iterator it = begin; it != end; ++it) {
        if (!is_first) res << ", ";
        if (f != nullptr) res << (*f)(*it);
        else res << *it;
        is_first = false;
    }
    res << aft;
    return res.str();
}

ostream &operator<<(ostream &out, __int128_t x) {
    if (x == 0) return out << 0;
    if (x < 0) out << '-', x = -x;
    string S;
    while (x) S.push_back('0' + x % 10), x /= 10;
    reverse(begin(S), end(S));
    return out << S;
}
ostream &operator<<(ostream &out, __uint128_t x) {
    if (x == 0) return out << 0;
    string S;
    while (x) S.push_back('0' + x % 10), x /= 10;
    reverse(begin(S), end(S));
    return out << S;
}
 
template <typename S, typename T>
ostream& operator <<(ostream& out, const pair<S, T>& p) {
    out << "{" << p.first << ", " << p.second << "}";
    return out;
}

template <typename... Args>
std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& a) {
    out << "{";
    std::apply([&out](auto&&... args) {
        size_t count = 0;
        ((out << (count++ ? ", " : "") << args), ...);
    }, a);
    return out << "}";
}

template<class T> 
ostream &operator<<(typename enable_if<!is_same<T, string>::value, ostream>::type &out, const T &arr){
	if(arr.empty()) return out << "{}";
	out << "{";
	for(auto it = arr.begin(); it != arr.end(); ++ it){
		out << *it;
		next(it) != arr.end() ? out << ", " : out << "}";
	}
	return out;
}

template<class Width, class Head>
void debugbin_out(Width w, Head H){
	for(auto rep = w; rep; -- rep, H >>= 1) cerr << (H & 1);
	cerr << endl;
}
template<class Width, class Head, class... Tail>
void debugbin_out(Width w, Head H, Tail... T){
	for(auto rep = w; rep; -- rep, H >>= 1) cerr << (H & 1);
	cerr << ", "; debugbin_out(w, T...);
}

#define debugbin(...) cerr << "[" << __LINE__ << "] " << "[" << #__VA_ARGS__ << "] ", debugbin_out(__VA_ARGS__)

void debug2_out() {}
template<class Head, class... Tail> 
void debug2_out(Head H, Tail... T) { 
    cerr << "\n"; for (auto x : H) cerr << x << ",\n"; debug2_out(T...); 
}

#define debug2(...) cerr << "[" << __LINE__ << "] " << "[" << #__VA_ARGS__ << "] ", debug2_out(__VA_ARGS__)

template<class TH>
void debug_impl(const char* name, TH val){
    cerr << name << ": " << val << endl;
}
template<class TH, class... TA>
void debug_impl(const char* names, TH curr_val, TA... vals) {
    while(*names != ',') cerr << *names++;
    cerr << ": " << curr_val << ", ";
    debug_impl(names+1, vals...);
}

#define debug(...) cerr << "[" << __LINE__ << "] ", debug_impl(#__VA_ARGS__, __VA_ARGS__)
} // namespace mitsuha


chrono::high_resolution_clock::time_point bgn;
void __attribute__((constructor)) _st_time() {
    bgn = chrono::high_resolution_clock::now();
}

void __attribute__((destructor)) _ed_time() {
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    string unit;
    #ifdef MILLI
    long long elapsed = chrono::duration_cast<chrono::milliseconds>(end - bgn).count();
    unit = "milli sec";
    #else
    long long elapsed = chrono::duration_cast<chrono::microseconds>(end - bgn).count();
    unit = "micro sec";
    #endif
    #ifndef STRESS
    #ifdef FASTIO
    print();
    print("Time Elapsed:", elapsed, unit);
    flush();
    #endif 
    #endif
}

#endif  // H_PRETTY_PRINT