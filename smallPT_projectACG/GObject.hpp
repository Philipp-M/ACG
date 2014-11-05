#ifndef GOBJECT_HPP
#define GOBJECT_HPP
#include "DataTypes.hpp"
#include "GPrimitiveObject.hpp"
/**
 * interface for more complex Objects, then primitves
 */
class GObject
{
public:
	virtual const GPrimitiveObject* intersect(const Ray& ray, double& t) const = 0;
	virtual Vec getCentroid() const = 0;
	virtual GBoundingBox createBoundingBox() const = 0;
	virtual ~GObject() {};
};

#endif // GOBJECT_HPP
