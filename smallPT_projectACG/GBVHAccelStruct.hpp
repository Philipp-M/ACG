#ifndef GBVHACCELSTRUCT_HPP_
#define GBVHACCELSTRUCT_HPP_

#include "DataTypes.hpp"
#include <algorithm>

class GBoundingBox
{
public:
	GBoundingBox(const Vec& min, const Vec& max)
	{
		this->min.x = std::min(max.x, min.x);
		this->min.y = std::min(max.y, min.y);
		this->min.z = std::min(max.z, min.z);
		this->max.x = std::max(max.x, min.x);
		this->max.y = std::max(max.y, min.y);
		this->max.z = std::max(max.z, min.z);
	}
	~GBoundingBox()
	{
	}
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
