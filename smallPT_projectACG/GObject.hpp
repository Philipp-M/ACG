#ifndef GOBJECT_HPP
#define GOBJECT_HPP
#include "DataTypes.hpp"
/**
 * interface for Geometric Objects
 */
class GObject
{
public:
	/**
	 * checks if the object was intersect by the ray, and if it was intersected,
	 * the intersection point, containing information about the intersection, like distance to intersection,
	 * intersection normal and color etc.
	 */
	virtual bool intersect(const Ray &ray, RayIntPt& intPoint) const = 0;
	/**
	 * gets the centroid out of the object, mostly necessary for the intersection structure BVH
	 */
	virtual Vec3 getCentroid() const = 0;
	/**
	 * Obviously creates a Axis-aligned Bounding Box around the Object
	 */
	virtual GBoundingBox createBoundingBox() const = 0;
	virtual ~GObject() {};

	//simple translation operations
	virtual void translate(const Vec3& t)  = 0;
	virtual void rotationX(float rad) = 0;
	virtual void rotationY(float rad)  = 0;
	virtual void rotationZ(float rad)  = 0;

	//very simple liner acceleration
	virtual void translateAcc(Vec3 t, double acc, long time) = 0;
};

#endif // GOBJECT_HPP
