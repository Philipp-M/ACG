#ifndef GACCELSTRUCT_HPP
#define GACCELSTRUCT_HPP

#include "DataTypes.hpp"

class GAccelStruct
{
public:
	/**
	 * checks if an object in the acceleration structure was intersected by the ray, and if it was intersected,
	 * the intersection point, containing information about the intersection, like distance to intersection,
	 * intersection normal and color etc.
	 */
	virtual bool intersect(const Ray &ray, RayIntPt& intPoint) const = 0;
	virtual ~GAccelStruct() {}
};

#endif // GACCELSTRUCT_HPP
