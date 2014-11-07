#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <cmath>
#include <algorithm>
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
	Vec origin, direction, invdir;
	Ray(Vec origin_, Vec direction_) :
			origin(origin_), direction(direction_)
	{
		invdir.x = 1.0/direction_.x;
		invdir.y = 1.0/direction_.y;
		invdir.z = 1.0/direction_.z;
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

// Bounding Box Primitive necessary for acceleration structures
class GBoundingBox
{
public:
	GBoundingBox(const Vec& min, const Vec& max)
	{
		double eps = 0.00000000023283064365386962891; // binary value 1/(1<<32)
		if((((((min.x) == min.y) == min.z) == max.x) == max.y) == max.z)
		{
			this->min = Vec();
			this->max = Vec();
		}
		else
		{
			this->min.x = std::min(max.x, min.x) - eps;
			this->min.y = std::min(max.y, min.y) - eps;
			this->min.z = std::min(max.z, min.z) - eps;
			this->max.x = std::max(max.x, min.x) + eps;
			this->max.y = std::max(max.y, min.y) + eps;
			this->max.z = std::max(max.z, min.z) + eps;
		}
	}
	~GBoundingBox()
	{
	}

	bool intersect(const Ray& ray, double& tmin, double& tmax) const
	{
		double t1 = (min.x - ray.origin.x)*ray.invdir.x;
		double t2 = (max.x - ray.origin.x)*ray.invdir.x;
		double t3 = (min.y - ray.origin.y)*ray.invdir.y;
		double t4 = (max.y - ray.origin.y)*ray.invdir.y;
		double t5 = (min.z - ray.origin.z)*ray.invdir.z;
		double t6 = (max.z - ray.origin.z)*ray.invdir.z;

		double tmin_ = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		double tmax_ = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
		// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
		if (tmax_ < 0)
		{
		    //tmin = tmax;
		    return false;
		}
		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin_ > tmax_)
		{
		    //t = tmax;
		    return false;
		}
		tmax = tmax_;
		tmin = tmin_;
		return true;
	}
	bool intersect(const Ray& ray, double& t) const
	{
		double t1 = (min.x - ray.origin.x) * ray.invdir.x;
		double t2 = (max.x - ray.origin.x) * ray.invdir.x;
		double t3 = (min.y - ray.origin.y) * ray.invdir.y;
		double t4 = (max.y - ray.origin.y) * ray.invdir.y;
		double t5 = (min.z - ray.origin.z) * ray.invdir.z;
		double t6 = (max.z - ray.origin.z) * ray.invdir.z;

		t = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
		// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
		if (tmax < 0 || t > tmax)
			return false;
		return true;
	}
	bool intersect(const Ray& ray) const
	{
		double t1 = (min.x - ray.origin.x) * ray.invdir.x;
		double t2 = (max.x - ray.origin.x) * ray.invdir.x;
		double t3 = (min.y - ray.origin.y) * ray.invdir.y;
		double t4 = (max.y - ray.origin.y) * ray.invdir.y;
		double t5 = (min.z - ray.origin.z) * ray.invdir.z;
		double t6 = (max.z - ray.origin.z) * ray.invdir.z;

		double tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
		// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
		if (tmax < 0 || tmin > tmax)
			return false;
		return true;
	}
	GBoundingBox operator+(const GBoundingBox &b) const
	{
		Vec minNew, maxNew;
		minNew.x = std::min(this->min.x, b.min.x);
		minNew.y = std::min(this->min.y, b.min.y);
		minNew.z = std::min(this->min.z, b.min.z);

		maxNew.x = std::max(this->max.x, b.max.x);
		maxNew.y = std::max(this->max.y, b.max.y);
		maxNew.z = std::max(this->max.z, b.max.z);
		return GBoundingBox(minNew,maxNew);
	}
	const Vec& getMax() const { return max; }
	const Vec& getMin() const { return min; }
	bool isEmpty() { return (((((min.x == min.y) == min.z) == max.x) == max.y) == max.z); } // ( aren't necesssary but compiler...
	friend std::ostream& operator<<(std::ostream &out, const GBoundingBox& b)
	{
		return out 	<< "(BBox: min:" << b.min << ", max: " << b.max << ")";
	}
private:
	Vec min, max; // 2 coordinates are enough for describing a Bounding Box
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
