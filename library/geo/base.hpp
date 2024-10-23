#ifndef AJAY_GEO_BASE
#define AJAY_GEO_BASE

namespace mitsuha{
template <typename T>
struct Point {
    T x, y;
    Point() : x(0), y(0) {}
    template <typename A, typename B>
    Point(A x, B y) : x(x), y(y) {}
    template <typename A, typename B>
    Point(pair<A, B> p) : x(p.fi), y(p.se) {}

    Point operator+=(const Point p) {
        x += p.x, y += p.y;
        return *this;
    }
    Point operator-=(const Point p) {
        x -= p.x, y -= p.y;
        return *this;
    }

    Point operator+(Point p) const { return {x + p.x, y + p.y}; }
    Point operator-(Point p) const { return {x - p.x, y - p.y}; }
    bool operator==(Point p) const { return x == p.x && y == p.y; }
    bool operator!=(Point p) const { return x != p.x || y != p.y; }
    Point operator-() const { return {-x, -y}; }
    Point operator*(T t) const { return {x * t, y * t}; }
    Point operator/(T t) const { return {x / t, y / t}; }

    bool operator<(Point p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }
    T dot(Point other) { return x * other.x + y * other.y; }
    T det(Point other) { return x * other.y - y * other.x; }

    double norm() { return sqrtl(x * x + y * y); }
    double angle() { return atan2(y, x); }

    Point rotate(double theta) {
        static_assert(!is_integral<T>::value);
        double c = cos(theta), s = sin(theta);
        return Point{c * x - s * y, s * x + c * y};
    }
};

template <typename T>
void rd(Point<T>& p) {
    io::rd(p.x), io::rd(p.y);
}
template <typename T>
void wt(Point<T>& p) {
    io::wt(p.x);
    io::wt(' ');
    io::wt(p.y);
}
template <typename T>
ostream &operator<<(ostream &out, const Point<T> &p){ return out << p.x << " " << p.y; }

// A -> B -> C +1 if it turns left, -1 if it turns right.
template <typename T>
int ccw(Point<T> A, Point<T> B, Point<T> C) {
    T x = (B - A).det(C - A);
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

template <typename REAL, typename T>
REAL dist(Point<T> A, Point<T> B) {
    A = A - B;
    T p = A.dot(A);
    return sqrt(REAL(p));
}

// ax+by+c
template <typename T>
struct Line {
    T a, b, c;

    Line(T a, T b, T c) : a(a), b(b), c(c) {}
    Line(Point<T> A, Point<T> B) {
        a = A.y - B.y, b = B.x - A.x, c = A.x * B.y - A.y * B.x;
    }
    Line(T x1, T y1, T x2, T y2) : Line(Point<T>(x1, y1), Point<T>(x2, y2)) {}

    template <typename U>
    U eval(Point<U> P) {
        return a * P.x + b * P.y + c;
    }
    template <typename U>
    T eval(U x, U y) {
        return a * x + b * y + c;
    }

    // Make the same straight line represented by the same a,b,c
    void normalize() {
        static_assert(is_same_v<T, int> || is_same_v<T, long long>);
        T g = gcd(gcd(abs(a), abs(b)), abs(c));
        a /= g, b /= g, c /= g;
        if (b < 0) { a = -a, b = -b, c = -c; }
        if (b == 0 && a < 0) { a = -a, b = -b, c = -c; }
    }

    // bool operator<(Line l) const {
    //     return tie(a, b, c) < tie(l.a, l.b, l.c);
    // }

    bool is_parallel(Line other) { return a * other.b - b * other.a == 0; }
    bool is_orthogonal(Line other) { return a * other.a + b * other.b == 0; }
};

template <typename T>
struct Segment {
    Point<T> A, B;

    Segment(Point<T> A, Point<T> B) : A(A), B(B) {}
    Segment(T x1, T y1, T x2, T y2)
            : Segment(Point<T>(x1, y1), Point<T>(x2, y2)) {}

    bool contain(Point<T> C) {
        static_assert(is_integral<T>::value);
        T det = (C - A).det(B - A);
        if (det != 0) return 0;
        return (C - A).dot(B - A) >= 0 && (C - B).dot(A - B) >= 0;
    }

    Line<T> to_Line() { return Line(A, B); }
};

template <typename REAL>
struct Circle {
    Point<REAL> O;
    REAL r;
    Circle(Point<REAL> O, REAL r) : O(O), r(r) {}
    Circle(REAL x, REAL y, REAL r) : O(x, y), r(r) {}
    template <typename T>
    bool contain(Point<T> p) {
        REAL dx = p.x - O.x, dy = p.y - O.y;
        return dx * dx + dy * dy <= r * r;
    }
};
} // namespace mitsuha
#endif // AJAY_GEO_BASE