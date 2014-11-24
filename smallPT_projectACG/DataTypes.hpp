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
struct Vec3
{        // Usage: time ./smallpt 5000 && xv image.ppm
	double x, y, z;                  // position, also color (r,g,b)
	Vec3(double x_ = 0, double y_ = 0, double z_ = 0)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	Vec3(const double vec[3])
	{
		x = vec[0];
		y = vec[1];
		z = vec[2];
	}
	Vec3(const float vec[3])
	{
		x = vec[0];
		y = vec[1];
		z = vec[2];
	}
	Vec3 operator+(const Vec3 &b) const
	{
		return Vec3(x + b.x, y + b.y, z + b.z);
	}
	Vec3 operator-(const Vec3 &b) const
	{
		return Vec3(x - b.x, y - b.y, z - b.z);
	}
	Vec3 operator*(double b) const
	{
		return Vec3(x * b, y * b, z * b);
	}
	Vec3 operator/(double b) const
	{
		return Vec3(x / b, y / b, z / b);
	}
	Vec3 mult(const Vec3 &b) const
	{
		return Vec3(x * b.x, y * b.y, z * b.z);
	}
	Vec3& operator*=(const Vec3& right)
	{
		this->x *= right.x;
		this->y *= right.y;
		this->z *= right.z;
		return *this;
	}
	Vec3& norm()
	{
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}
	double dot(const Vec3 &b) const
	{
		return x * b.x + y * b.y + z * b.z;
	} // cross:
	Vec3 operator%(const Vec3& b) const
	{
		return Vec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
	friend std::ostream& operator<<(std::ostream &out, const Vec3& v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
};
struct Vec2
{
	double x, y;
	Vec2(double x_ = 0, double y_ = 0)
	{
		x = x_;
		y = y_;
	}
	Vec2 operator+(const Vec2 &b) const
	{
		return Vec2(x + b.x, y + b.y);
	}
	Vec2 operator-(const Vec2 &b) const
	{
		return Vec2(x - b.x, y - b.y);
	}
	Vec2 operator*(double b) const
	{
		return Vec2(x * b, y * b);
	}
	Vec2 operator/(double b) const
	{
		return Vec2(x / b, y / b);
	}
	Vec2 mult(const Vec2 &b) const
	{
		return Vec2(x * b.x, y * b.y);
	}
	Vec2& norm()
	{
		return *this = *this * (1 / sqrt(x * x + y * y));
	}
	double dot(const Vec3 &b) const
	{
		return x * b.x + y * b.y;
	}
	friend std::ostream& operator<<(std::ostream &out, const Vec2& v)
	{
		return out << "(" << v.x << ", " << v.y << ")";
	}
};
// Ray STRUCTURE
struct Ray
{
	Vec3 origin, direction, invdir;
	Ray(Vec3 origin_, Vec3 direction_) :
			origin(origin_), direction(direction_)
	{
		invdir.x = 1.0/direction_.x;
		invdir.y = 1.0/direction_.y;
		invdir.z = 1.0/direction_.z;
	}
	void setDirection(const Vec3& dir)
	{
		direction = dir;
		invdir.x = 1.0/dir.x;
		invdir.y = 1.0/dir.y;
		invdir.z = 1.0/dir.z;
	}
	friend std::ostream& operator<<(std::ostream &out, const Ray& r)
	{
		return out << "(origin: " << r.origin << ", direction: " << r.direction << ")";
	}
};
// ENUM OF MATERIAL TYPES USED IN radiance FUNCTION
enum Refl_t
{
	DIFF, SPEC, REFR, GLOSS, EMIS
};

// Bounding Box Primitive necessary for acceleration structures
class GBoundingBox
{
public:
	GBoundingBox(const Vec3& min, const Vec3& max)
	{
		double eps = 0.00000000023283064365386962891; // binary value 1/(1<<32)
		if((((((min.x) == min.y) == min.z) == max.x) == max.y) == max.z)
		{
			this->min = Vec3();
			this->max = Vec3();
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
		Vec3 minNew, maxNew;
		minNew.x = std::min(this->min.x, b.min.x);
		minNew.y = std::min(this->min.y, b.min.y);
		minNew.z = std::min(this->min.z, b.min.z);

		maxNew.x = std::max(this->max.x, b.max.x);
		maxNew.y = std::max(this->max.y, b.max.y);
		maxNew.z = std::max(this->max.z, b.max.z);
		return GBoundingBox(minNew,maxNew);
	}
	const Vec3& getMax() const { return max; }
	const Vec3& getMin() const { return min; }
	bool isEmpty() { return (((((min.x == min.y) == min.z) == max.x) == max.y) == max.z); } // ( aren't necesssary but compiler...
	friend std::ostream& operator<<(std::ostream &out, const GBoundingBox& b)
	{
		return out 	<< "(BBox: min:" << b.min << ", max: " << b.max << ")";
	}
private:
	Vec3 min, max; // 2 coordinates are enough for describing a Bounding Box
};

struct RayIntPt
{
	Vec3 position;
	Vec3 normal;
	Vec3 calcNormal;
	Vec3 color;
	Vec3 emission;
	double glossyRoughness;
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
