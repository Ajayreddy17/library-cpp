#ifndef AJAY_CONTAINER_UTILS
#define AJAY_CONTAINER_UTILS

namespace mitsuha{
template <typename T> int count_leq(const vector<T> &v, const T &key) { return upper_bound(v.begin(), v.end(), key) - v.begin();  }
template <typename T> int count_lt(const vector<T> &v, const T &key) { return lower_bound(v.begin(), v.end(), key) - v.begin();  }
template <typename T> int count_geq(const vector<T> &v, const T &key) { return v.size() - count_lt(v, key); }
template <typename T> int count_gt(const vector<T> &v, const T &key) { return v.size() - count_leq(v, key); }

template <typename Container, typename Key>
auto min_geq(const Container &container, const Key &key) -> decltype(make_optional(*container.lower_bound(key))) {
    if (auto it = container.lower_bound(key); it == container.end()) return nullopt;
    else return make_optional(*it);
}
template <typename Container, typename Key>
auto min_gt(const Container &container, const Key &key) -> decltype(make_optional(*container.upper_bound(key))) {
    if (auto it = container.upper_bound(key); it == container.end()) return nullopt;
    else return make_optional(*it);
}
template <typename Container, typename Key>
auto max_leq(const Container &container, const Key &key) -> decltype(make_optional(*container.upper_bound(key))) {
    if (auto it = container.upper_bound(key); it == container.begin()) return nullopt;
    else return make_optional(*--it);
}
template <typename Container, typename Key>
auto max_lt(const Container &container, const Key &key) -> decltype(make_optional(*container.lower_bound(key))) {
    if (auto it = container.lower_bound(key); it == container.begin()) return nullopt;
    else return make_optional(*--it);
}
template <typename T>
optional<T> min_geq(const vector<T> &v, const T &key) {
    auto it = lower_bound(v.begin(), v.end(), key);
    return it == v.end() ? nullopt : make_optional(*it);
}
template <typename T>
optional<T> min_gt(const vector<T> &v, const T &key) {
    auto it = upper_bound(v.begin(), v.end(), key);
    return it == v.end() ? nullopt : make_optional(*it);
}
template <typename T>
optional<T> max_leq(const vector<T> &v, const T &key) {
    auto it = upper_bound(v.begin(), v.end(), key);
    return it == v.begin() ? nullopt : make_optional(*--it);
}
template <typename T>
optional<T> max_lt(const vector<T> &v, const T &key) {
    auto it = lower_bound(v.begin(), v.end(), key);
    return it == v.begin() ? nullopt : make_optional(*--it);
}
} // namespace mitsuha
#endif // AJAY_CONTAINER_UTILS