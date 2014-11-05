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
	virtual bool intersect(const Ray &ray, RayIntPt& intPoint) const = 0;
	virtual Vec getCentroid() const = 0;
	virtual GBoundingBox createBoundingBox() const = 0;
	virtual ~GObject() {};
};

#endif // GOBJECT_HPP
