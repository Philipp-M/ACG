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
	bool intersect(const Ray& ray, double& t) const
	{
		float t1 = (min.x - ray.origin.x)*ray.invdir.x;
		float t2 = (max.x - ray.origin.x)*ray.invdir.x;
		float t3 = (min.y - ray.origin.y)*ray.invdir.y;
		float t4 = (max.y - ray.origin.y)*ray.invdir.y;
		float t5 = (min.z - ray.origin.z)*ray.invdir.z;
		float t6 = (max.z - ray.origin.z)*ray.invdir.z;
		float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
		// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
		if (tmax < 0)
		{
		    t = tmax;
		    return false;
		}
		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
		    t = tmax;
		    return false;
		}
		t = tmin;
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
