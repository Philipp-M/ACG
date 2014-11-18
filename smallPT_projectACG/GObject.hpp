#ifndef GOBJECT_HPP
#define GOBJECT_HPP
#include "DataTypes.hpp"
/**
 * interface for more complex Objects, then primitves
 */
class GObject
{
public:
	virtual bool intersect(const Ray &ray, RayIntPt& intPoint) const = 0;
	virtual Vec3 getCentroid() const = 0;
	virtual GBoundingBox createBoundingBox() const = 0;
	virtual ~GObject() {};
};

#endif // GOBJECT_HPP
