#ifndef GACCELSTRUCT_HPP
#define GACCELSTRUCT_HPP

#include "DataTypes.hpp"
#include "GPrimitiveObject.hpp"

class GAccelStruct
{
public:
	/**
	 * returns NULL if nothing was hit, else the pointer to the corresponding intersection Object
	 */
	virtual bool intersect(const Ray &ray, RayIntPt& intPoint) const = 0;
	GAccelStruct(){}
	virtual ~GAccelStruct() {}
};

#endif // GACCELSTRUCT_HPP
