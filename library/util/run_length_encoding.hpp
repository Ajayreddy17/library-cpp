#ifndef AJAY_RUN_LENGTH_ENCODING
#define AJAY_RUN_LENGTH_ENCODING

namespace mitsuha{
template <typename C>
vector<pair<typename C::value_type, int>> run_length_encoding(C& S) {
    using T = typename C::value_type;
    if (S.empty()) return {};
    vector<pair<T, int>> ret;
    T c = S[0];
    int n = 1;
    for (int i = 1; i < (int)S.size(); i++) {
        if (S[i] == c)
            n++;
        else {
            ret.emplace_back(c, n);
            c = S[i], n = 1;
        }
    }
    ret.emplace_back(c, n);
    return ret;
}
} // namespace mitsuha
#endif // AJAY_RUN_LENGTH_ENCODING
