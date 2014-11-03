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
	virtual ~GObject() {};

	//simple translation operatoins
	virtual void translate(const Vec& t)  = 0;
	virtual void rotationX(float rad) = 0;
	virtual void rotationY(float rad)  = 0;
	virtual void rotationZ(float rad)  = 0;

	//very simple liner acceleration
	virtual void translateAcc(Vec t, double acc, long time) = 0;
};

#endif // GOBJECT_HPP
