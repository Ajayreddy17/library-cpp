#ifndef AJAY_COUNT_SUBSEQ
#define AJAY_COUNT_SUBSEQ

namespace mitsuha{
// https://noshi91.hatenablog.com/entry/2023/02/26/135340
template <typename mint, typename STRING>
mint count_subseq(STRING& S) {
    map<int, mint> dp;
    mint sm = 1;
    for (auto&& x: S) {
        auto it = dp.find(x);
        if (it == dp.end()) {
            dp[x] = sm;
            sm += sm;
        } else {
            mint d = (*it).second;
            (*it).second = sm;
            sm += sm - d;
        }
    }
    return sm;
}
} // namespace mitsuha
#endif // AJAY_COUNT_SUBSEQ