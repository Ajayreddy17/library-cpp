#ifndef AJAY_INTERVAL_INTERSECTION
#define AJAY_INTERVAL_INTERSECTION

namespace mitsuha {
template <typename T>
vector<pair<T, T>> interval_intersection(vector<pair<T, T>> a, vector<pair<T, T>> b,
                                         bool is_sorted = false, bool disjoint = false) {
    auto normalize = [&](vector<pair<T, T>>& v) {
        if (!is_sorted) sort(v.begin(), v.end());
        vector<pair<T, T>> res;
        for (auto [l, r] : v) {
            assert(l <= r);
            if (l == r) continue;
            if (res.empty() || res.back().second < l) {
                res.push_back({l, r});
            } else {
                if (res.back().second > l) {
                    res.back().second = std::max(res.back().second, r);
                } else {
                    res.push_back({l, r});
                }
            }
        }
        return res;
    };

    if (not disjoint) {
        a = normalize(a);
        b = normalize(b);
        is_sorted = true;
    }
    if (not is_sorted){
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
    }

    vector<pair<T, T>> res;
    int i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        T l = max(a[i].first, b[j].first);
        T r = min(a[i].second, b[j].second);
        if (l < r) res.push_back({l, r});
        if (a[i].second < b[j].second) ++i;
        else ++j;
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_INTERVAL_INTERSECTION