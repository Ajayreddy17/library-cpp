#ifndef AJAY_INTERVAL_UNION
#define AJAY_INTERVAL_UNION

namespace mitsuha{
// Given a set of intervals, return the list of sorted disjoint intervals with the same union
vector<pair<int, int>> interval_union(vector<pair<int, int>> a, bool merge_adjacent = true, bool is_sorted = false){
    if(!is_sorted) sort(a.begin(), a.end());
    for(auto [l, r]: a) assert(l <= r);
    vector<pair<int, int>> res;
    for(auto [l, r]: a){
        if(res.empty() || res.back().second < l || !merge_adjacent && res.back().second == l) res.push_back({l, r});
        else res.back().second = max(res.back().second, r);
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_INTERVAL_UNION