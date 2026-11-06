#ifndef AJAY_RESTORE_PATH
#define AJAY_RESTORE_PATH

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/shortest_path/restore_path.hpp
vector<int> restore_path(vector<int> par, int t){
    vector<int> pth = {t};
    while (par[pth.back()] != -1) {
        pth.emplace_back(par[pth.back()]);
    }
    reverse(pth.begin(), pth.end());
    return pth;
}
} // namespace mitsuha
#endif // AJAY_RESTORE_PATH