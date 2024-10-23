namespace mitsuha{

#define Int(...) int __VA_ARGS__; IN(__VA_ARGS__)
#define Ll(...)  long long __VA_ARGS__; IN(__VA_ARGS__)
#define Str(...) std::string __VA_ARGS__; IN(__VA_ARGS__)
#define Chr(...) char __VA_ARGS__; IN(__VA_ARGS__)
#define Dbl(...) long double __VA_ARGS__; IN(__VA_ARGS__)
#define Vt(type, name, size) std::vector<type> name(size); read(name)
#define Vvt(type, name, h, w) std::vector<std::vector<type>> name(h, std::vector<type>(w)); read(name)

void read(int &a) { std::cin >> a; }
void read(long long &a) { std::cin >> a; }
void read(char &a) { std::cin >> a; }
void read(double &a) { std::cin >> a; }
void read(long double &a) { std::cin >> a; }
void read(std::string &a) { std::cin >> a; }
template <class T, class S>
void read(std::pair<T, S> &p) {
    read(p.first), read(p.second);
}
template <class T>
void read(std::vector<T> &a) {
  for (auto &i: a) read(i);
}
template <class T>
void read(T &a) {
    std::cin >> a;
}
void IN() {}
template <class Head, class... Tail>
void IN(Head &head, Tail &... tail) {
    read(head);
    IN(tail...);
}
 
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &A) {
    os << A.first << " " << A.second;
    return os;
}
 
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &A) {
    for (size_t i = 0; i < A.size(); i++) {
        if (i) os << " ";
        os << A[i];
    }
    return os;
}
 
// chatgpt helped me
class CoutInitializer {
public:
    CoutInitializer() { std::cout << std::fixed << std::setprecision(15); }
};
static CoutInitializer cout_initializer;
 
void print() {
    std::cout << "\n";
    std::cout.flush();
}
 
template <class Head, class... Tail>
void print(Head &&head, Tail &&... tail) {
    std::cout << head;
    if (sizeof...(Tail)) std::cout << " ";
    print(std::forward<Tail>(tail)...);
}
} // namespace mitsiha