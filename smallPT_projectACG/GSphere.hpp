#ifndef GSPHERE_HPP
#define GSPHERE_HPP

#include <iostream>
#include "DataTypes.hpp"
#include "GBVHAccelStruct.hpp"
#include "GObject.hpp"

struct GSphere: public GObject
{
	double radius;
	Vec3 position, emission, color;
	Refl_t refl;
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
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	Vec3 getCentroid() const;

	//simple translation operations
	void translate(const Vec3& t);
	void rotationX(float rad);
	void rotationY(float rad);
	void rotationZ(float rad);

	//very simple linear acceleration
	void translateAcc(Vec3 t, double acc, long time);

	void rotation(float rad, const Vec3& dir);
	GBoundingBox createBoundingBox() const;
};

#endif //GSPHERE_HPP
