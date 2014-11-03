#ifndef GSPHERE_HPP
#define GSPHERE_HPP

#include <iostream>
#include "DataTypes.hpp"
#include "GPrimitiveObject.hpp"
#include "GObject.hpp"

class GSphere: public GPrimitiveObject , public GObject
{
	double radius;       // radius
	Vec position, emission, color;      // position, emission, color
	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive)
	GSphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_) :
			radius(rad_), position(p_), emission(e_), color(c_), refl(refl_)
	{
	}
	~GSphere() {};

	/***** implementations of GPrimitiveObject *****/
	/*
	 * implemented intersection method of a sphere
	 */
	double intersect(const Ray &ray) const;
	Vec getNorm(const Vec& position) const;
	Vec getColor(const Vec& position) const;
	Vec getEmission(const Vec& position) const;
	Refl_t getReflectionType() const;
	/***** implementations of GObject *****/
	/**
	 * for simplicity the sphere is also a "normal" Object
	 */
	const GPrimitiveObject* intersect(const Ray& ray, double& t) const;
	Vec getCentroid() const;

	//simple translation operatoins
	void translate(Vec t);
	void rotationX(float rad);
	void rotationY(float rad);
	void rotationZ(float rad);

	//very simple liner acceleration
	void translateAcc(Vec t, double acc, long time);

	void rotation(float rad, Vec dir);
};

#endif //GSPHERE_HPP
