#ifndef AJAY_QUADRATIC_EQUATION
#define AJAY_QUADRATIC_EQUATION

namespace mitsuha{
// returns roots in sorted order
vector<double> quadratic_equation(double a, double b, double c, const double eps = 1e-8) {
    if (abs(a) < eps) {
        if (abs(b) < eps) {
            assert(abs(c) >= eps);
            return {};
        }
        return {-c / b};
    }
    double d = b * b - 4 * a * c;
    if (d <= -eps) return {};
    if (d < eps) return {-0.5 * b / a};
    d = sqrt(d);
    const double x1 = -0.5 * (b >= 0 ? b + d : b - d) / a, x2 = c / (x1 * a);
    return x1 > x2 ? vector<double>{x2, x1} : vector<double>{x1, x2};
}
} // namespace mitsuha
#endif // AJAY_QUADRATIC_EQUATION