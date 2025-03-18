#ifndef AJAY_TIMER
#define AJAY_TIMER

namespace mitsuha{
struct Timer {
    bool started;
    chrono::high_resolution_clock::time_point s;

    Timer() : started(false) {}
    void start() {
        started = true;
        s = chrono::high_resolution_clock::now();
    }

    // second from start
    double operator()() {
        assert(started);
        chrono::high_resolution_clock::time_point t = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = t - s;
        return diff.count();
    }
};
} // namespace mitsuha
#endif // AJAY_TIMER