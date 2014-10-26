#ifndef GPRIMITIVEOBJECT_HPP
#define GPRIMITIVEOBJECT_HPP

#include "DataTypes.hpp"

/**
 * interface for simple Objects like a sphere or a triangle
 */
class GPrimitiveObject
{
public:
	/**
	 * returns the distance from the origin to the intersection,
	 * if there was no intersection it returns 0
	 */
	virtual double intersect(const Ray &ray) const = 0;
	/**
	 * returns a unit vector presenting the normal of the surface at the given position,
	 * if the position is not on the Object the result is undefined
	 */
	virtual Vec getNorm(const Vec& position) const = 0;
	/**
	 * returns the color of the surface at the given position,
	 * if the position is not on the Object the result is undefined
	 */
	virtual Vec getColor(const Vec& position) const = 0;
	/**
	 * returns the emission of the surface at the given position,
	 * if the position is not on the Object the result is undefined
	 */
	virtual Vec getEmission(const Vec& position) const = 0;
	/**
	 * returns the type of reflection(e.g. diffuse etc.)
	 */
	virtual Refl_t getReflectionType() const = 0;
	/**
	 * returns the centroid of the Object
	 */
	Vec getCentroid() const;
	virtual ~GPrimitiveObject() {}
};

#endif // GPRIMITIVEOBJECT_HPP
