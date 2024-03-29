#ifndef AJAY_SOLVE_LINEAR_DIOPHANTINE
#define AJAY_SOLVE_LINEAR_DIOPHANTINE

namespace mitsuha{
// solutions to ax + by = c where x in [xlow, xhigh) and y in [ylow, yhigh)
// cnt, leftsol, rightsol, gcd of a and b
// O(log(min(abs(a), abs(b))))
template<class T>
optional<array<T, 6>> linear_diophantine(T a, T b, T c, T xlow, T xhigh, T ylow, T yhigh){
	assert(xlow <= xhigh && ylow <= yhigh);
	if(xlow == xhigh || ylow == yhigh) return {};
	if(a == 0 && b == 0){
		if(c) return {};
		return array<T, 6>{(xhigh - xlow) * (yhigh - ylow), xlow, ylow, xhigh - 1, yhigh - 1, 0};
	}
	if(a == 0){
		if(c % b) return {};
		return array<T, 6>{xhigh - xlow, xlow, c / b, xhigh - 1, c / b, b >= 0 ? b : -b};
	}
	if(b == 0){
		if(c % a) return {};
		return array<T, 6>{yhigh - ylow, c / a, ylow, c / a, yhigh - 1, a >= 0 ? a : -a};
	}
	T x = 1, y = 0, g = a >= 0 ? a : -a;
	for(T _b = b >= 0 ? b : -b, q, r, u = 0, v = 1; _b; ){
		q = g / _b, r = g % _b;
		tie(g, _b, x, y, u, v) = tuple{_b, r, u, v, x - u * q, y - v * q};
	}
	if(c % g) return {};
	-- xhigh, -- yhigh;
	x *= c / g, y *= c / g;
	if(a < 0) x = -x;
	if(b < 0) y = -y;
	a /= g, b /= g, c /= g;
	auto shift = [](T &x, T &y, T a, T b, T cnt){ x += cnt * b, y -= cnt * a; };
	int sign_a = a > 0 ? 1 : -1, sign_b = b > 0 ? 1 : -1;
	shift(x, y, a, b, (xlow - x) / b);
	if(x < xlow) shift(x, y, a, b, sign_b);
	if(x > xhigh) return {};
	T lx1 = x;
	shift(x, y, a, b, (xhigh - x) / b);
	if(x > xhigh) shift(x, y, a, b, -sign_b);
	T rx1 = x;
	shift(x, y, a, b, -(ylow - y) / a);
	if(y < ylow) shift(x, y, a, b, -sign_a);
	if(y > yhigh) return {};
	T lx2 = x;
	shift(x, y, a, b, -(yhigh - y) / a);
	if(y > yhigh) shift(x, y, a, b, sign_a);
	T rx2 = x;
	if(lx2 > rx2) swap(lx2, rx2);
	T lx = max(lx1, lx2), rx = min(rx1, rx2);
	if(lx > rx) return {};
	return array<T, 6>{(rx - lx) / (b >= 0 ? b : -b) + 1, lx, (c - lx * a) / b, rx, (c - rx * a) / b, g};
}
} // namespace mitsuha
#endif // AJAY_SOLVE_LINEAR_DIOPHANTINE