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

	//simple translation operatoins
	virtual void translate(const Vec3& t)  = 0;
	virtual void rotationX(float rad) = 0;
	virtual void rotationY(float rad)  = 0;
	virtual void rotationZ(float rad)  = 0;

	//very simple liner acceleration
	virtual void translateAcc(Vec3 t, double acc, long time) = 0;
};

#endif // GOBJECT_HPP
