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
	virtual const GPrimitiveObject * intersect(const Ray &ray, double& t) const = 0;

	virtual ~GAccelStruct() {}
};

#endif // GACCELSTRUCT_HPP
