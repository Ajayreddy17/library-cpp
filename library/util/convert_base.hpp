#ifndef AJAY_CONVERT_BASE
#define AJAY_CONVERT_BASE

namespace mitsuha{
template<typename T>
vector<int> convert_base(T x, int base) {
    vector<int> ret;
    do {
        ret.push_back(x % base);
        x /= base;
    } while (x > 0);
    reverse(ret.begin(), ret.end());
    return ret;
}
} // namespace mitsuha
#endif // AJAY_CONVERT_BASE