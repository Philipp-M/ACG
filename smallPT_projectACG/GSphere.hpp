#ifndef GSPHERE_HPP
#define GSPHERE_HPP

#include <iostream>
#include "DataTypes.hpp"
#include "GBVHAccelStruct.hpp"
#include "GObject.hpp"

struct GSphere: public GObject
{
	double radius;       // radius
	Vec3 position, emission, color;      // position, emission, color
	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive)
	GSphere(double rad_, Vec3 p_, Vec3 e_, Vec3 c_, Refl_t refl_) :
			radius(rad_), position(p_), emission(e_), color(c_), refl(refl_)
	{
	}
	~GSphere() {};

	Vec3 getNorm(const Vec3& position) const;
	Vec3 getColor() const;
	Vec3 getEmission() const;
	Refl_t getReflectionType() const;
	/***** implementations of GObject *****/
	/**
	 * for simplicity the sphere is also a "normal" Object
	 */
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	Vec3 getCentroid() const;
	GBoundingBox createBoundingBox() const;
};

#endif //GSPHERE_HPP
