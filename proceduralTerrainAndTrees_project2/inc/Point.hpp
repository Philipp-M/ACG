#pragma once

template<typename T>
struct Point2 {
	Point2(T x, T y) : x(x), y(y) {};
	T x, y;
	inline bool operator ==(const Point2<T> &r) const { return (r.x == x && r.y == y); }
};
typedef Point2<double> Point2d;
typedef Point2<float> Point2f;
