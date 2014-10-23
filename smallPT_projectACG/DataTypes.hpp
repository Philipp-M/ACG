/*
 * DataTypes.hpp
 *
 *  Created on: 22.10.2014
 *      Author: Philipp Mildenberger
 */

#ifndef DATATYPES_HPP_
#define DATATYPES_HPP_

#include <cmath>
/* Basic Data types: Vec - 2D Vector and all the standard operations
 *                   Ray - a Ray with an origin and a direction(unit) vector
 *                   Refl_t - an enum, describing the Reflection type
 *                   *********************** custom Types ******************
 *                   RayIntPt - a ray intersection point contains information
 *                              about the intersection with a geometric body:
 *                                  Normal of Point(unit vector),
 *                                  Color of Reflection,
 *                                  Type of Reflection,
 *                                  and of course the intersection point
 *                                  for performance issues, also the distance
 *                                  from the origin of the ray to the intersection point is included
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
	Vec origin, direction;
	Ray(Vec origin_, Vec direction_) :
			origin(origin_), direction(direction_)
	{
	}
};
// ENUM OF MATERIAL TYPES USED IN radiance FUNCTION
enum Refl_t
{
	DIFF, SPEC, REFR
};
//struct RayIntPt
//{
//	Vec position;
//	Vec normal;
//	Vec color;
//	Vec emission;
//	double distance;
//
//	Refl_t reflType;
//
//	RayIntPt(Vec position_, Vec normal_, double distance_, Vec color_, Vec emission_, Refl_t reflType_) :
//			normal(normal_), position(position_), distance(distance_), color(color_), emission(emission_), reflType(reflType_)
//	{
//	}
//};

#endif /* DATATYPES_HPP_ */
