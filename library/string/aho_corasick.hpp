#ifndef AJAY_AHO_CORASICK
#define AJAY_AHO_CORASICK

namespace mitsuha{
template <typename T = char>
struct Aho_Corasick {
    using value_type = T;

    Aho_Corasick() : _next(1) {}

    template <typename Container, enable_if_t<is_constructible_v<value_type, typename Container::value_type>, nullptr_t> = nullptr>
    void add(const Container& s) {
        int cur = init_state();
        for (value_type c : s) {
            auto [it, inserted] = _next[cur].try_emplace(c, _next.size());
            if (inserted) _next.emplace_back();
            cur = it->second;
        }
        _marks.push_back(cur);
        _built = false;
    }

    void build() {
        _built = true;
        const int n = _next.size();
        _failure.resize(n, 0);
        _count.resize(n, 0);
        for (int mark : _marks) ++_count[mark];
        deque<int> dq{ 0 };
        while (dq.size()) {
            const int cur = dq.front();
            dq.pop_front();
            for (const auto& [c, nxt] : _next[cur]) {
                if (cur) {
                    _failure[nxt] = next_state(_failure[cur], c);
                    _count[nxt] += _count[_failure[nxt]];
                }
                dq.push_back(nxt);
            }
        }
    }

    int init_state() const {
        return 0;
    }
    int next_state(int state, value_type c) const {
        assert(_built);
        while (true) {
            if (auto it = _next[state].find(c); it == _next[state].end()) {
                if (state == 0) return 0;
                state = _failure[state];
            } else {
                return it->second;
            }
        }
    }
    int count_suffix_matching(int state) const {
        assert(_built);
        return _count[state];
    }
private:
    vector<int> _failure;
    vector<map<value_type, int>> _next;
    vector<int> _marks;
    vector<int> _count;
    bool _built = true;
};
} // namespace mitsuha
#endif // AJAY_AHO_CORASICK