#ifndef AJAY_POP
#define AJAY_POP

namespace mitsuha{
template <typename T>
T POP(deque<T> &que) { T a = que.front(); que.pop_front(); return a; }
template <typename T, typename Container, typename Compare>
T POP(priority_queue<T, Container, Compare> &que) { T a = que.top(); que.pop(); return a; }
template <typename T>
T POP(vector<T> &que) { T a = que.back(); que.pop_back(); return a; }
} // namespace mitsuha
#endif // AJAY_POP