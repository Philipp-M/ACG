#ifndef GBVHACCELSTRUCT_HPP_
#define GBVHACCELSTRUCT_HPP_

#include "DataTypes.hpp"
#include <algorithm>
class GBoundingVolume
{
public:
	virtual bool intersect(const Ray& ray) const = 0;
	virtual ~GBoundingVolume();
};

class GBoundingBox: GBoundingVolume
{
public:
	GBoundingBox(const Vec& min, const Vec& max) : min(min), max(max) {}
	bool intersect(const Ray& ray) const
	{
		double tmin = (min.x - ray.origin.x) / ray.direction.x;
		double tmax = (max.x - ray.origin.x) / ray.direction.x;
		if (tmin > tmax)
			std::swap(tmin, tmax);
		double tymin = (min.y - ray.origin.y) / ray.direction.y;
		double tymax = (max.y - ray.origin.y) / ray.direction.y;
		if (tymin > tymax)
			std::swap(tymin, tymax);
		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;
		double tzmin = (min.z - ray.origin.z) / ray.direction.z;
		double tzmax = (max.z - ray.origin.z) / ray.direction.z;
		if (tzmin > tzmax)
			std::swap(tzmin, tzmax);
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		return true;
	}
private:
	Vec min, max; // 2 coordinates are enough for describing a Bounding Box
};

class GBVHAccelStruct
{
public:
	GBVHAccelStruct();
	virtual ~GBVHAccelStruct();
};

#endif /* GBVHACCELSTRUCT_HPP_ */
