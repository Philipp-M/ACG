
#ifndef GOBJECT_HPP_
#define GOBJECT_HPP_
#include "DataTypes.hpp"
#include "GPrimitiveObject.hpp"

/**
 * interface for more complex Objects, then primitves
 */
class GObject
{
public:
	virtual const GPrimitiveObject* intersect(const Ray& ray, double& t) const = 0;
	virtual Vec getCentroid()  = 0;
	virtual ~GObject() {};
};

#endif /* GOBJECT_HPP_ */
