#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <cmath>
#include <iostream>
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
	friend std::ostream& operator<<(std::ostream &out, const Vec& v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
};
// Ray STRUCTURE
struct Ray
{
	Vec origin, direction;
	Ray(Vec origin_, Vec direction_) :
			origin(origin_), direction(direction_)
	{
	}
	friend std::ostream& operator<<(std::ostream &out, const Ray& r)
	{
		return out << "(origin: " << r.origin << ", direction: " << r.direction << ")";
	}
};
// ENUM OF MATERIAL TYPES USED IN radiance FUNCTION
enum Refl_t
{
	DIFF, SPEC, REFR
};

struct RayIntPt
{
	Vec position;
	Vec normal;
	Vec color;
	Vec emission;
	double distance;
	Refl_t reflType;
	friend std::ostream& operator<<(std::ostream &out, const RayIntPt& i)
	{
		return out 	<< "(position: " << i.position << ", normal: " << i.normal << ", distance: "
					<< i.distance << ", color: " << i.color << ", emission: " << i.emission
					<< ", reflType: " << i.reflType << ")";
	}
};
#endif // DATATYPES_HPP
