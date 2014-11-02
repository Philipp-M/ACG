#ifndef GBVHACCELSTRUCT_HPP_
#define GBVHACCELSTRUCT_HPP_

#include "DataTypes.hpp"

class GBoundingVolume
{
public:
	virtual bool intersect(const Ray& ray) const = 0;
	virtual ~GBoundingVolume();
};

class GBoundingBox : GBoundingVolume
{
public:
	bool intersect(const Ray& ray) const;

};

class GBVHAccelStruct
{
public:
	GBVHAccelStruct();
	virtual ~GBVHAccelStruct();
};

#endif /* GBVHACCELSTRUCT_HPP_ */
