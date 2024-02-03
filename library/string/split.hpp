#ifndef AJAY_SPLIT
#define AJAY_SPLIT

namespace mitsuha{
vector<string> split(string S, char sep = ',') {
    vector<string> res = {""};
    for (auto &&s: S) {
        if (s == sep) res.emplace_back("");
        else res.back() += s;
    }
    return res;
}

vector<string> split(string S, string seps = " ,") {
    vector<string> res = {""};
    for (auto &&s: S) {
        if (count(seps.begin(), seps.end(), s)) res.emplace_back("");
        else res.back() += s;
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_SPLIT