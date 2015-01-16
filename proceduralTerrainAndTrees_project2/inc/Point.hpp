#pragma once
#include <cmath>
#include <iostream>
const double EPSILON = 1e-5;

template<typename T>
struct Point2 {
	Point2(T x, T y) : x(x), y(y) {};
	T x, y;
	bool operator ==(const Point2<T> &r) const
	{
		return ((1-EPSILON)*r.x <= x && (1+EPSILON)*r.x>=r.x && (1-EPSILON)*r.y <= y && (1+EPSILON)*r.y >= r.y);
	}
	Point2<T> operator-(const Point2<T> &r) const { return Point2<T>(x-r.x, y-r.y); }
	T length() const { return sqrt(x*x+y*y); }

	friend std::ostream& operator<<(std::ostream& os, const Point2<T>& dt) { return os << '(' << dt.x << ", " << dt.y << ')';}
};
typedef Point2<double> Point2d;
typedef Point2<float> Point2f;
