#ifndef AJAY_IS_SUBSEQ
#define AJAY_IS_SUBSEQ

namespace mitsuha{
template <typename STRING>
bool is_subseq(STRING& S, STRING& T) {
    int p = 0;
    for (auto&& s: S) {
        while (p < len(T) && T[p] != s) ++p;
        if (p == len(T)) return false;
        ++p;
    }
    return true;
}
} // namespace mitsuha
#endif // AJAY_IS_SUBSEQ