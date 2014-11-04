#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <cmath>
/* Basic Data types: Vec - 2D Vector and all the standard operations
 *                   Ray - a Ray with an origin and a direction(unit) vector
 *                   Refl_t - an enum, describing the Reflection type
 */

// Vec STRUCTURE ACTS AS POINTS, COLORS, VECTORS
struct Vec
{        // Usage: time ./smallpt 5000 && xv image.ppm
	double x, y, z;                  // position, also color (r,g,b)
	Vec(double x_ = 0, double y_ = 0, double z_ = 0)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	Vec operator+(const Vec &b) const
	{
		return Vec(x + b.x, y + b.y, z + b.z);
	}
	Vec operator-(const Vec &b) const
	{
		return Vec(x - b.x, y - b.y, z - b.z);
	}
	Vec operator*(double b) const
	{
		return Vec(x * b, y * b, z * b);
	}
	Vec mult(const Vec &b) const
	{
		return Vec(x * b.x, y * b.y, z * b.z);
	}
	Vec& norm()
	{
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}
	double dot(const Vec &b) const
	{
		return x * b.x + y * b.y + z * b.z;
	} // cross:
	Vec operator%(const Vec& b) const
	{
		return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};
// Ray STRUCTURE
struct Ray
{
	Vec origin, direction, invdir;
	Ray(Vec origin_, Vec direction_) :
			origin(origin_), direction(direction_)
	{
		invdir.x = 1.0/direction_.x;
		invdir.y = 1.0/direction_.y;
		invdir.z = 1.0/direction_.z;
	}
};
// ENUM OF MATERIAL TYPES USED IN radiance FUNCTION
enum Refl_t
{
	DIFF, SPEC, REFR
};
#endif // DATATYPES_HPP
